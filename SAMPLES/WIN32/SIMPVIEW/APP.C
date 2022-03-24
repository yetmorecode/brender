/*
 * app.c
 *
 * (C) Copyright Argonaut Software. 1994-1995.  All rights reserved.
 *
 * Simple World Viewer - Application code
 */
#include <windows.h>
#include <windowsx.h>
#include <assert.h>

#include <brender.h>

#include "app.h"
#include "dispatch.h"
#include "world.h"
#include "buffer.h"
#include "resource.h"

/*
 * Single world and single view
 */
struct brwin_world *AppWorld;
struct brwin_view *AppView;

/*
 * Flag, true when application is active
 */
int AppIsForeground = FALSE;

/*
 * Application wide logical palette
 */
HPALETTE AppPalette;

/*
 * Appliation name and title from resources
 */
char AppName[9];
char AppTitle[41];

/*
 * Handle to application's instance
 */
HINSTANCE AppInstance;

/*
 * Forward declarations
 */
static void BR_CALLBACK BrWinWarning(char *message);
static void BR_CALLBACK BrWinError(char *message);
static void MakeRange(br_colour a, br_colour d, br_colour s, PALETTEENTRY *pe, int npe);
static br_colour MakeColour(br_colour a, br_colour d, br_colour s,float level);
static HPALETTE CreateAppPalette(void);

/**
 ** Brender diagnostic handler that uses Windows
 **/
br_diaghandler BrWinErrorHandler = {
	"Windows Diagnostic Handler",
	BrWinWarning,
	BrWinError,
};

/*
 * WinMain()
 *
 * Windows Entry point
 */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int CmdShow)
{
    MSG msg;
    HANDLE hAccelTable;

	/*
	 * Start up application - exit if initialisation failed
	 */
	 if (!ApplicationBegin(hInstance, CmdShow))
		  return FALSE;

	/*
	 * Load up shortcut table
	 */
    hAccelTable = LoadAccelerators(AppInstance, AppName);

    /*
	 * Grab messages and process them until WM_QUIT happens
	 */
    for (;;) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT)
				break;
	        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
					TranslateMessage(&msg);
            	DispatchMessage(&msg);
        	}
    	} else {
    		if (IdleProcess())
    			WaitMessage();
		}
  	}

	/*
	 * Close down and global application data
	 */
	ApplicationEnd();

	/*
	 * Returns the value from PostQuitMessage
	 */
    return msg.wParam; 
}

/*
 * ApplicationBegin()
 *
 * Application specific startup - return TRUE if completed OK
 */
BOOL ApplicationBegin(HINSTANCE hInstance, int CmdShow)
{
	int i;

	 /*
     * Load the application name and description strings.
	 */
    LoadString(hInstance, IDS_APPNAME,		AppName,	sizeof(AppName));
    LoadString(hInstance, IDS_DESCRIPTION,	AppTitle,	sizeof(AppTitle));

	/*
 	 * Remember our instance handle
	 */
   	AppInstance = hInstance;

	/*
 	 * Register main window class
	 */
	if(MainWindowRegister() == FALSE) {
		 MessageBox (NULL,"Could not register window class",
			"ERROR", MB_OK | MB_TASKMODAL);
		return FALSE;
	}

	/*
	 * Initialise the buffering classes
	 */
	BufferClassesInit();

	/*
	 * Make sure at least one sort of buffering is available
	 */
	for(i=0; i < BUFFER_COUNT; i++)
		if(BufferClasses[i].available)
			break;

	if(i >= BUFFER_COUNT) {
	    MessageBox (NULL,"No off-screen buffer types available",
	    	"ERROR", MB_OK | MB_TASKMODAL); 
		return FALSE;
	}

	/*
	 * Initialise the renderer
	 */
	BrBegin();
	BrErrorHandlerSet(&BrWinErrorHandler);
	BrZbBegin(BR_PMT_INDEX_8,BR_PMT_DEPTH_16);

	/*
	 * Build a logical palette
	 */
    AppPalette = CreateAppPalette();

	/*
	 * Initialise the 3D world
	 */
	AppWorld = WorldAllocate();

	/*
	 * Create a view into the 3D world
	 */
	AppView = WindowViewAllocate(AppWorld, AppPalette, CmdShow);

	if(AppView == NULL) {
	    MessageBox (NULL,"Could not create view",
	    	"ERROR", MB_OK | MB_TASKMODAL); 
		return FALSE;
	}

    return TRUE;
}

/*
 * ApplicationEnd()
 *
 * Close down rendering libraries
 */
void ApplicationEnd(void)
{
    BrZbEnd();
    BrEnd();
}

/*
 * IdleProcess()
 *
 * Called by WinMain() when no messages waiting
 *
 * Returns TRUE if there is no more idle processing to be done
 */
BOOL IdleProcess(void)
{
	/*
	 * Don't do anything if world and view not setup yet
	 * not been setup yet
	 */
	if( AppWorld == NULL ||
		AppView == NULL ||
		AppView->buffer == NULL)
		return TRUE;

	switch(AppWorld->update) {
	case WORLD_UPDATE_NEVER:
		return TRUE;

	case WORLD_UPDATE_FOREGROUND:
		if(!AppIsForeground)
			return TRUE;

	 	/* FALL THROUGH */
	case WORLD_UPDATE_ALWAYS:

		WorldUpdate(AppWorld);

		ViewRender(AppView);
		ViewScreenUpdate(AppView, NULL, 0);
		ViewFrameRateUpdate(AppView);
	}
	return FALSE;
}

/*
 * WindowViewAllocate()
 *
 * Create a new window and it's associated view into the world
 */
brwin_view *WindowViewAllocate(brwin_world *world, HPALETTE palette, int CmdShow)
{
	brwin_view *view;
	int i;

	/*
	 * Allocate new view structure
	 */
	view = GlobalAllocPtr(GHND,sizeof(*view));

	/*
	 * Set buffer type to first available
	 */
	for(i=0; i < BUFFER_COUNT; i++)
		if(BufferClasses[i].available)
			break;

	assert( i < BUFFER_COUNT);

	view->blit_type = i;
	view->force_all = 0;
	view->frame_count = 0;
	view->start_time = timeGetTime();
	view->buffer = NULL;
	view->palette = AppPalette;
	view->title = AppTitle;
	view->world = world;

	/*
	 * Create the window - pass view pointer through lpvParam
	 */
    view->window = CreateWindow(AppName, AppTitle,
						WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_CLIPCHILDREN,
                        CW_USEDEFAULT, 0,  DEFAULT_VIEW_WIDTH, DEFAULT_VIEW_HEIGHT,
                        NULL,NULL, AppInstance, view );

    if(view->window == NULL)
        return NULL;

	/*
	 * Get a global device context onto window that is used throughout application
	 * Window class has CS_OWNDC set
	 */
    view->screen = GetDC(view->window);

	view->palette = palette;

	/*
	 * Make the window appear - this will generate a WM_SIZE message that will
	 * create the off screen buffer and pixelmaps
	 */
    ShowWindow(view->window, CmdShow);

	return view;
}

/*
 * WindowViewFree()
 *
 * Destroy a view and the window if it is still attached
 */
void WindowViewFree(brwin_view *view)
{
	/*
	 * Detroy window if it is still attached
	 */
	if(view->window) {
		/*
		 * Detach view
		 */
		INST_VIEW_SET(view->window,NULL);
		DestroyWindow(view->window);
	}

	/*
	 * Release view memory
	 */
	GlobalFreePtr(view);
}

/*
 * MakeRange()
 *
 * Fill a given array of PALETTEENTRYs with a ramp of colours
 * using MakeColour()
 */
static void MakeRange(br_colour a, br_colour d, br_colour s,
						PALETTEENTRY *pe, int npe)
{
	int i;
	br_colour colour;

	for(i=0; i < npe; i++) {
		colour = MakeColour(a,d,s, i/ (float)npe);
		pe[i].peRed = (BYTE)BR_RED(colour);
		pe[i].peGreen = (BYTE)BR_GRN(colour);
		pe[i].peBlue = (BYTE)BR_BLU(colour);
	}
}

/*
 * MakeColour()
 *
 * Generate a shaded colour at 'level' along a ramp from
 *
 * 0                    0.75       1.0
 * ambient -----------> diffuse -> specular 
 */
static br_colour MakeColour(br_colour a, br_colour d, br_colour s,float level)
{

	if(level < 0.75f) {
		level /= 0.75f;

		return BR_COLOUR_RGB(
			(1.0-level) * BR_RED(a) + level * BR_RED(d),
			(1.0-level) * BR_GRN(a) + level * BR_GRN(d),
			(1.0-level) * BR_BLU(a) + level * BR_BLU(d));
	} else {
		level = (level-0.75f)/0.25f;

		return BR_COLOUR_RGB(
			(1.0-level) * BR_RED(d) + level * BR_RED(s),
			(1.0-level) * BR_GRN(d) + level * BR_GRN(s),
			(1.0-level) * BR_BLU(d) + level * BR_BLU(s));
	}
}


/*
 * CreateAppPalette()
 *
 * Build the aplication wide logical palette
 */
static HPALETTE CreateAppPalette(void)
{
    HPALETTE 		palette;
	LOGPALETTE		*logical_palette;
	PALETTEENTRY	*pe;
	HDC				screen_dc;
	int				i;

	/*
	 * Build the logical palette
	 */
	logical_palette = GlobalAllocPtr(GHND, sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * PALETTE_SIZE));
	logical_palette->palVersion    = 0x300;
	logical_palette->palNumEntries = PALETTE_SIZE;
	pe = logical_palette->palPalEntry;

	/*
	 * Fill the palette with system colours
	 */
	screen_dc = GetDC(NULL);
	GetSystemPaletteEntries(screen_dc, 0, PALETTE_SIZE, pe);
	ReleaseDC(NULL, screen_dc);

	/*
	 * Mark all entries as NOCOLLAPSE
	 */
	for(i=10; i< PALETTE_SIZE-10; i++)
		pe[i].peFlags = PC_NOCOLLAPSE;

	/*
	 * Clear all the non system colours
	 */
	for(i=10; i< PALETTE_SIZE-10; i++) {
		pe[i].peRed = 255;
		pe[i].peGreen = 0;
		pe[i].peBlue = 255;
	}

	/*
	 * Fill in our own ranges
	 */
	pe = logical_palette->palPalEntry+10;

	MakeRange(BR_COLOUR_RGB(0,0,0),BR_COLOUR_RGB(147,147,147),BR_COLOUR_RGB(255,255,255), pe+  0, 32);
	MakeRange(BR_COLOUR_RGB(0,0,0),BR_COLOUR_RGB( 60, 60,238),BR_COLOUR_RGB(255,255,255), pe+ 32, 32);
	MakeRange(BR_COLOUR_RGB(0,0,0),BR_COLOUR_RGB( 60,238, 60),BR_COLOUR_RGB(255,255,255), pe+ 64, 32);
	MakeRange(BR_COLOUR_RGB(0,0,0),BR_COLOUR_RGB( 60,238,238),BR_COLOUR_RGB(255,255,255), pe+ 96, 32);
	MakeRange(BR_COLOUR_RGB(0,0,0),BR_COLOUR_RGB(238, 60, 60),BR_COLOUR_RGB(255,255,255), pe+128, 32);
	MakeRange(BR_COLOUR_RGB(0,0,0),BR_COLOUR_RGB(238, 60,238),BR_COLOUR_RGB(255,255,255), pe+160, 32);
	MakeRange(BR_COLOUR_RGB(0,0,0),BR_COLOUR_RGB(238,238, 60),BR_COLOUR_RGB(255,255,255), pe+192, 32);

	/*
	 * create the palette
	 */
	palette = CreatePalette(logical_palette);

	GlobalFreePtr(logical_palette);

	return palette;
}

/*
 * AboutDlgProc()
 *
 * Procedure for about dialog box - could use dispatch system, but this is
 * pretty trivial
 */
BOOL CALLBACK AboutDlgProc( HWND hwnd, unsigned msg, UINT wparam, LONG lparam )
{
    switch(msg) {
    case WM_INITDIALOG:
		return TRUE;

    case WM_COMMAND:
    	if(LOWORD( wparam ) == IDOK) {
	    	EndDialog( hwnd, TRUE);
	    	return TRUE;
		}
		break;
    }
    return FALSE;
}

/*
 * BRender Error handling functions
 */
static void BR_CALLBACK BrWinWarning(char *message)
{
    MessageBox(NULL, message, "WARNING", MB_OK | MB_TASKMODAL); 
	ExitProcess(10);
}

static void BR_CALLBACK BrWinError(char *message)
{
    MessageBox(NULL, message, "ERROR", MB_OK | MB_TASKMODAL);
	ExitProcess(10);
}
