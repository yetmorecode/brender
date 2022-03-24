/*
 * app.h
 *
 * (C) Copyright Argonaut Software. 1994.  All rights reserved.
 *
 * Windows App specific declarations
 */

/*
 * IDs of strings in string table
 */
#define IDS_APPNAME     1
#define IDS_DESCRIPTION 2

/*
 * Are common contols used
 */
#define HAS_STATUSBAR 0
#define HAS_TOOLBAR 0

#define IDC_TOOLBAR		1000
#define IDC_STATUSBAR	1001

/*
 * Commands associated with world update types
 */
#define ID_WORLD_UPDATE				2000
#define ID_WORLD_UPDATE_ALWAYS		ID_WORLD_UPDATE+WORLD_UPDATE_ALWAYS
#define ID_WORLD_UPDATE_FOREGROUND	ID_WORLD_UPDATE+WORLD_UPDATE_FOREGROUND
#define ID_WORLD_UPDATE_NEVER		ID_WORLD_UPDATE+WORLD_UPDATE_NEVER

/*
 * Commands associated with buffer types
 */
#define ID_UPDATE					2010
#define ID_UPDATE_DIBSECTION		ID_UPDATE+BUFFER_DIBSECTION
#define ID_UPDATE_WING				ID_UPDATE+BUFFER_WING
#define ID_UPDATE_STRETCHDIBITS		ID_UPDATE+BUFFER_STRETCHDIBITS
#define ID_UPDATE_DUMMY				ID_UPDATE+BUFFER_DUMMY


/*
 * Single world and single view
 */
extern struct brwin_world *AppWorld;
extern struct brwin_view *AppView;

/*
 * Flag, true when application is active
 */
extern int AppIsForeground;

/*
 * Handle to application's instance
 */
extern HINSTANCE AppInstance;

/*
 * Application wide logical palette
 */
extern HPALETTE AppPalette;

/*
 * Appliation name and title from resources
 */
extern char AppName[];
extern char AppTitle[];

/*
 * Width and height of new view windows
 */
#define DEFAULT_VIEW_WIDTH	552
#define DEFAULT_VIEW_HEIGHT 348

/*
 * Constants for palette depth
 */
#define PALETTE_SIZE	256
#define BIT_COUNT		8

/*
 * Prototypes
 */

/*
 * app.c
 */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
BOOL ApplicationBegin(HINSTANCE hInstance, int CmdShow);
void ApplicationEnd(void);

BOOL IdleProcess(void);

struct brwin_view *WindowViewAllocate(struct brwin_world *world, HPALETTE palette, int CmdShow);
void WindowViewFree(struct brwin_view *view);

BOOL CALLBACK AboutDlgProc( HWND hwnd, unsigned msg, UINT wparam, LONG lparam);

/*
 * main.c
 */

BOOL MainWindowRegister(void);
/*
 * Macros for accessing extra instance data
 */
#define INST_VIEW_SET(hwnd,view) SetWindowLong((hwnd),0,(LONG)(view))
#define INST_VIEW_GET(hwnd) ((brwin_view *)GetWindowLong((hwnd),0))

