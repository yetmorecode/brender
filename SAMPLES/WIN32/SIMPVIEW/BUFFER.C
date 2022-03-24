/*
 * buffer.c
 *
 * (C) Copyright Argonaut Software. 1995.  All rights reserved.
 *
 * Managing an off screen buffer and copying it to the screen
 *
 */
#include <windows.h>
#include <windowsx.h>
#include <assert.h>

#include "resource.h"
#include "buffer.h"
#include "app.h"

#if 0
#define DIB_USAGE DIB_RGB_COLORS
#else
#define DIB_USAGE DIB_PAL_COLORS
#endif

/*
 * Forward declarations
 */
static int BufferDIBSectionInit(void);
static brwin_buffer *BufferDIBSectionAllocate(HDC screen, HPALETTE *palette, int width, int height);
static void BufferDIBSectionFree(brwin_buffer *gbuf);
static void BufferDIBSectionBlit(brwin_buffer *gbuf, HDC screen, int dest_x, int dest_y,
	int	src_x,	int src_y,int width, int height);

static int BufferStretchDIBitsInit(void);
static brwin_buffer *BufferStretchDIBitsAllocate(HDC screen, HPALETTE *palette, int width, int height);
static void BufferStretchDIBitsFree(brwin_buffer *gbuf);
static void BufferStretchDIBitsBlit(brwin_buffer *gbuf, HDC screen, int dest_x, int dest_y,
	int	src_x,	int src_y,int width, int height);

static int BufferWinGInit(void);
static brwin_buffer *BufferWinGAllocate(HDC screen, HPALETTE *palette, int width, int height);
static void BufferWinGFree(brwin_buffer *gbuf);
static void BufferWinGBlit(brwin_buffer *gbuf, HDC screen, int dest_x, int dest_y,
	int	src_x,	int src_y,int width, int height);

static int BufferDummyInit(void);
static brwin_buffer *BufferDummyAllocate(HDC screen, HPALETTE *palette, int width, int height);
static void BufferDummyFree(brwin_buffer *gbuf);
static void BufferDummyBlit(brwin_buffer *gbuf, HDC screen, int dest_x, int dest_y,
	int	src_x,	int src_y,int width, int height);

/**
 ** Table of classes
 **/
brwin_buffer_class BufferClasses[] = {
	{ BufferWinGInit,			BufferWinGAllocate, 			}, /* BUFFER_WING			*/
	{ BufferStretchDIBitsInit,	BufferStretchDIBitsAllocate,	}, /* BUFFER_STRETCHDIBBITS	*/
	{ BufferDIBSectionInit,		BufferDIBSectionAllocate, 		}, /* BUFFER_DIBSECTION		*/
	{ BufferDummyInit,			BufferDummyAllocate,			}, /* BUFFER_DUMMY			*/
};

/*
 * Macro to convert a number of bits to longword aligned bytes
 */
#define WIDTHBYTES(bits) ((((bits) + 31) & ~31) >> 3)

/*
 * BufferClassesInit()
 *
 * Initialise class table
 *
 * Returns the number of avialable classes
 */
int BufferClassesInit(void)
{
	int i,count = 0;

	assert(BUFFER_COUNT == sizeof(BufferClasses)/sizeof(BufferClasses[0]));

	for(i=0; i< BUFFER_COUNT ; i ++)
		if((BufferClasses[i].available = BufferClasses[i].init()) != 0)
			count++;

	return count;
}

/**
 ** DIB Section
 **/
struct brwin_buffer_cds {
	struct brwin_buffer b;
	HBITMAP bitmap;
};

/*
 * Local pointer to CreateDIBSection function
 */
static FARPROC CreateDIBSection_P;
#define CreateDIBSection CreateDIBSection_P

/*
 * BufferInitDIBSection
 *
 * Per app. initialisation of the DIBsection type of buffering
 *
 * Return true if this type is useable
 */
static int BufferDIBSectionInit(void)
{
	HMODULE hgdi;
	/*
	 * look up the CreatDIBSection function in GDI32
	 */
	hgdi = GetModuleHandle("GDI32");

	if(hgdi == NULL)
		return FALSE;

	CreateDIBSection_P = (void *)GetProcAddress(hgdi, "CreateDIBSection");

	return CreateDIBSection_P != NULL;
}

static brwin_buffer *BufferDIBSectionAllocate(HDC screen, HPALETTE *palette, int width, int height)
{
	struct brwin_buffer_cds *buf;
	int i;
	struct {
		BITMAPINFOHEADER header;
		RGBQUAD			colours[PALETTE_SIZE];
	} info;
#if DIB_USAGE == DIB_RGB_COLORS
	PALETTEENTRY palentries[PALETTE_SIZE];
#else
	short *index;
#endif

	/*
	 * Create the buffer struct
	 */
	buf = GlobalAllocPtr(GHND,sizeof(*buf));

	/*
	 * Create the bitmap info and palette
	 */
	memset(&info,0,sizeof(info));
	info.header.biSize = sizeof(BITMAPINFOHEADER);
	info.header.biWidth = width;
	info.header.biHeight = height;
	info.header.biPlanes = 1;
	info.header.biBitCount = BIT_COUNT;
	info.header.biCompression = BI_RGB;
	info.header.biSizeImage = WIDTHBYTES(width * BIT_COUNT) * height;
	info.header.biClrUsed = 256;

#if DIB_USAGE == DIB_RGB_COLORS
	GetPaletteEntries(palette,0,PALETTE_SIZE, palentries);

	for(i = 0; i < PALETTE_SIZE; i++) {
		info.colours[i].rgbRed = palentries[i].peRed;
		info.colours[i].rgbGreen = palentries[i].peGreen;
		info.colours[i].rgbBlue = palentries[i].peBlue;
	}
#else
	index = (short *)info.colours;

	for(i = 0; i < PALETTE_SIZE; i++)
		index[i] = i;
#endif

	/*
	 * Create the DIB section
	 */
	buf->bitmap = (HBITMAP)CreateDIBSection(screen, (LPBITMAPINFO)&info, DIB_USAGE, &buf->b.bits, 0, 0);

	if(buf->bitmap == NULL) {
		GlobalFreePtr(buf);
		return NULL;
	}

	buf->b.width = width;
	buf->b.height = height;
	buf->b.blit = BufferDIBSectionBlit;
	buf->b.free = BufferDIBSectionFree;

	return (brwin_buffer *)buf;
}

static void BufferDIBSectionFree(brwin_buffer *gbuf)
{
	struct brwin_buffer_cds *buf = (struct brwin_buffer_cds *)gbuf;
	/*
	 * Free the DIB section
	 */
	DeleteObject(buf->bitmap);
	/*
	 * Free the  buffer
	 */
	GlobalFreePtr(buf);
}

static void BufferDIBSectionBlit(brwin_buffer *gbuf, HDC screen, int dest_x, int dest_y,
	int	src_x,	int src_y,int width, int height)
{
	struct brwin_buffer_cds *buf = (struct brwin_buffer_cds *)gbuf;
	HDC buffer;
	HBITMAP old_bitmap;

    buffer = CreateCompatibleDC(screen);
	old_bitmap = SelectObject(buffer,buf->bitmap);

	BitBlt(screen,dest_x,dest_y,width,height,buffer,src_x,src_y,SRCCOPY);

	SelectObject(buffer, old_bitmap);
	DeleteDC(buffer);

	GdiFlush();
}


/**
 ** StretchDIBits
 **/
struct brwin_buffer_sdb {
	struct brwin_buffer b;
	BITMAPINFOHEADER *info;
};

/*
 * Per app. initialisation of the DIBsection type of buffering
 */
static int BufferStretchDIBitsInit(void)
{
	/*
	 * Always available
	 */
	return TRUE;
}

/*
 * Allocate an off screen buffer for use with StretchDIBits()
 */
static brwin_buffer *BufferStretchDIBitsAllocate(HDC screen, HPALETTE *palette, int width, int height)
{
	struct brwin_buffer_sdb *buf;
	int i;
#if DIB_USAGE == DIB_RGB_COLORS
	PALETTEENTRY palentries[PALETTE_SIZE];
	RGBQUAD *colours;
#else
	short *index;
#endif


	/*
	 * Create the buffer struct
	 */
	buf = GlobalAllocPtr(GHND,sizeof(*buf));

	/*
	 * Allocate the bitmap info, colour table, and pixels
	 */
	buf->info = GlobalAllocPtr(GHND,sizeof(BITMAPINFOHEADER)
								+ PALETTE_SIZE * sizeof(RGBQUAD)
								+ WIDTHBYTES(width * BIT_COUNT) * height);

	buf->b.bits = ((char *)buf->info + sizeof(BITMAPINFOHEADER)
								+ PALETTE_SIZE * sizeof(RGBQUAD));

	buf->info->biSize = sizeof(BITMAPINFOHEADER);
	buf->info->biWidth = width;
	buf->info->biHeight = height;
	buf->info->biPlanes = 1;
	buf->info->biBitCount = BIT_COUNT;
	buf->info->biCompression = BI_RGB;
	buf->info->biSizeImage = WIDTHBYTES(width * BIT_COUNT) * height;
	buf->info->biClrUsed = 256;

#if DIB_USAGE == DIB_RGB_COLORS
	/*
	 * Make the colour table
	 */
	colours = (RGBQUAD *)((char *)buf->info+sizeof(BITMAPINFOHEADER));

	GetPaletteEntries(palette,0,PALETTE_SIZE, palentries);

	for(i = 0; i < PALETTE_SIZE; i++) {
		colours[i].rgbRed = palentries[i].peRed;
		colours[i].rgbGreen = palentries[i].peGreen;
		colours[i].rgbBlue = palentries[i].peBlue;
	}
#else
	index = (short *)((char *)buf->info+sizeof(BITMAPINFOHEADER));

	for(i = 0; i < PALETTE_SIZE; i++)
		index[i] = i;
#endif

	/*
	 * Fill in buffer structure 
	 */
	buf->b.width = width;
	buf->b.height = height;
	buf->b.blit = BufferStretchDIBitsBlit;
	buf->b.free = BufferStretchDIBitsFree;

	return (brwin_buffer *)buf;
}

/*
 * Release a buffer that was previously allocated with
 * BufferStretchDIBitsAllocate()
 */
static void BufferStretchDIBitsFree(brwin_buffer *gbuf)
{
	struct brwin_buffer_sdb *buf = (struct brwin_buffer_sdb *)gbuf;

	/*
	 * Free the bitmap
	 */
	GlobalFreePtr(buf->info);

	/*
	 * Free the buffer
	 */
	GlobalFreePtr(buf);
}

/*
 * Copy buffer to window
 */
static void BufferStretchDIBitsBlit(brwin_buffer *gbuf, HDC screen, int dest_x, int dest_y,
	int	src_x, int src_y,int width, int height)
{
	struct brwin_buffer_sdb *buf = (struct brwin_buffer_sdb *)gbuf;

	StretchDIBits(screen,
			dest_x,dest_y,width,height,
			src_x,buf->b.height-src_y-height,width,height,
			buf->b.bits,
			(BITMAPINFO *)buf->info,
			DIB_USAGE,
			SRCCOPY);

	GdiFlush();
}


/**
 ** WinG
 **/
struct brwin_buffer_wing {
	struct brwin_buffer b;
	HDC dc;
	HBITMAP bitmap;
	HBITMAP old_bitmap;
};

/*
 * Table of pointers to WinG functions
 */
static struct {
	char *name;
	FARPROC ptr;
} WinGFuncs[] = {
	{ "WinGCreateDC" },
	{ "WinGCreateBitmap" },
	{ "WinGRecommendDIBFormat" },
	{ "WinGBitBlt" },
	{ "WinGStretchBlt" },
};

#define WinGCreateDC			(WinGFuncs[0].ptr)
#define WinGCreateBitmap		(WinGFuncs[1].ptr)
#define WinGRecommendDIBFormat	(WinGFuncs[2].ptr)
#define WinGBitBlt				(WinGFuncs[3].ptr)
#define WinGStretchBlt			(WinGFuncs[4].ptr)

static int BufferWinGInit(void)
{
	char sys_path[MAX_PATH+10];
	HMODULE hwing;
	int i,n;
	
	/*
	 * See if wing32.dll is in system directory
	 */
	n = GetSystemDirectory(sys_path,sizeof(sys_path));

	if(n != 0 && sys_path[n-1] != '/' && sys_path[n-1] != '\\')
		sys_path[n++] = '\\';

	strcpy(sys_path+n,"wing32.dll");

	n = GetFileAttributes(sys_path);

	if(n == 0xFFFFFFFF)
		return FALSE;

	/*
	 * look up the various WinG functions in WING32.DLL
	 */
	hwing = LoadLibrary("WING32");

	if(hwing == NULL)
		return FALSE;

	for(i=0; i < sizeof(WinGFuncs)/sizeof(WinGFuncs[0]); i++) {
		 WinGFuncs[i].ptr = (void *)GetProcAddress(hwing, WinGFuncs[i].name);
		 if(WinGFuncs[i].ptr == NULL)
		 	return FALSE;
	}

	return TRUE;
}

static brwin_buffer *BufferWinGAllocate(HDC screen, HPALETTE *palette, int width, int height)
{
	PALETTEENTRY palentries[PALETTE_SIZE];
	struct brwin_buffer_wing *buf;
	struct {
		BITMAPINFOHEADER header;
		RGBQUAD			colours[PALETTE_SIZE];
	} info;
	int i;

	/*
	 * Create the buffer struct
	 */
	buf = GlobalAllocPtr(GHND,sizeof(*buf));

	/*
	 * Create the WING Context
	 */
	buf->dc = (HDC)WinGCreateDC();

	/*
	 * Create the bitmap info and palette
	 */
	memset(&info,0,sizeof(info));
	info.header.biSize = sizeof(BITMAPINFOHEADER);
	info.header.biWidth = width;
	info.header.biHeight = height;
	info.header.biPlanes = 1;
	info.header.biBitCount = BIT_COUNT;
	info.header.biCompression = BI_RGB;
	info.header.biSizeImage = WIDTHBYTES(width * BIT_COUNT) * height;
	info.header.biClrUsed = 256;

	GetPaletteEntries(palette,0,PALETTE_SIZE, palentries);

	for(i = 0; i < PALETTE_SIZE; i++) {
		info.colours[i].rgbRed = palentries[i].peRed;
		info.colours[i].rgbGreen = palentries[i].peGreen;
		info.colours[i].rgbBlue = palentries[i].peBlue;
	}

	/*
	 * Create the bitmap and select it into the DC
	 */
	buf->bitmap = (HBITMAP)WinGCreateBitmap(buf->dc, (LPBITMAPINFO)&info, &buf->b.bits);

	if(buf->bitmap == NULL) {
		GlobalFreePtr(buf);
		return NULL;
	}

	buf->old_bitmap = SelectObject(buf->dc, buf->bitmap);

	/*
	 * Fill in the generic part of the buffer structure
	 */
	buf->b.width = width;
	buf->b.height = height;
	buf->b.blit = BufferWinGBlit;
	buf->b.free = BufferWinGFree;

	return (brwin_buffer *)buf;
}

static void BufferWinGFree(brwin_buffer *gbuf)
{
	struct brwin_buffer_wing *buf = (struct brwin_buffer_wing *)gbuf;

	SelectObject(buf->dc,buf->old_bitmap);

	/*
	 * Free the bitmap
	 */
	DeleteObject(buf->bitmap);

	/*
	 * Free the DC
	 */
	DeleteDC(buf->dc);

	/*
	 * Free the  buffer
	 */
	GlobalFreePtr(buf);
}

static void BufferWinGBlit(brwin_buffer *gbuf, HDC screen, int dest_x, int dest_y,
	int	src_x,	int src_y,int width, int height)
{
	struct brwin_buffer_wing *buf = (struct brwin_buffer_wing *)gbuf;

	WinGBitBlt(screen, dest_x, dest_y, width, height, buf->dc, src_x, src_y);
	GdiFlush();
}

/**
 ** Dummy buffering - does not copy to the screen at all
 **/
/*
 * Per app. initialisation of the DIBsection type of buffering
 */
static int BufferDummyInit(void)
{
	/*
	 * Always available
	 */
	return TRUE;
}

/*
 * Allocate an off screen buffer for use with Dummy()
 */
static brwin_buffer *BufferDummyAllocate(HDC screen, HPALETTE *palette, int width, int height)
{
	struct brwin_buffer *buf;

	/*
	 * Create the buffer struct
	 */
	buf = GlobalAllocPtr(GHND,sizeof(*buf));

	/*
	 * Allocate the bitmap 
	 */
	buf->bits = GlobalAllocPtr(GHND,WIDTHBYTES(width * BIT_COUNT) * height);


	/*
	 * Fill in buffer structure 
	 */
	buf->width = width;
	buf->height = height;
	buf->blit = BufferDummyBlit;
	buf->free = BufferDummyFree;

	return (brwin_buffer *)buf;
}

/*
 * Release a buffer that was previously allocated with
 * BufferDummyAllocate()
 */
static void BufferDummyFree(brwin_buffer *buf)
{
	/*
	 * Free the bitmap
	 */
	GlobalFreePtr(buf->bits);

	/*
	 * Free the buffer
	 */
	GlobalFreePtr(buf);
}

/*
 * Copy buffer to window
 */
static void BufferDummyBlit(brwin_buffer *buf, HDC screen, int dest_x, int dest_y,
	int	src_x, int src_y,int width, int height)
{
	/* Do nothing */
}
