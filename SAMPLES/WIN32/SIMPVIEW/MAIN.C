/*
 * main.c
 *
 * (C) Copyright Argonaut Software. 1994-1995.  All rights reserved.
 *
 * Simple World Viewer - Main Window message and command handlers 
 */
#include <windows.h>
#include <windowsx.h>
#include <assert.h>

#include "app.h"
#include "dispatch.h"
#include "world.h"
#include "buffer.h"
#include "resource.h"

/*
 * Forward declarations
 */
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);

static LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
static LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
static LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
static LRESULT MsgMouseMove(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
static LRESULT MsgMouseDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
static LRESULT MsgMouseUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
static LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam);
static LRESULT MsgActivateApp(HWND hwnd, UINT uMessage, WPARAM fActive, LPARAM dwThreadID);
static LRESULT MsgPaint(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam);
static LRESULT MsgPalette(HWND hwnd, UINT uMessage, WPARAM hwndPalChg, LPARAM lParam);

static LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl);
static LRESULT CmdSetBlitType(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl);
static LRESULT CmdSetWorldUpdate(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl);
static LRESULT CmdToggleForceAll(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl);
static LRESULT CmdAbout(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl);

/*
 * Main window message dispatch table definition.
 */
static MSD MainMsgFunctions[] = {
	{WM_CREATE,				MsgCreate},
    {WM_COMMAND, 			MsgCommand},
    {WM_DESTROY, 			MsgDestroy},
    {WM_MOUSEMOVE,  		MsgMouseMove},
    {WM_LBUTTONDOWN,  		MsgMouseDown},
    {WM_LBUTTONUP,  		MsgMouseUp},
	{WM_SIZE, 				MsgSize},
	{WM_ACTIVATEAPP,		MsgActivateApp},
	{WM_PAINT,				MsgPaint},
	{WM_QUERYNEWPALETTE,	MsgPalette},
	{WM_PALETTECHANGED,		MsgPalette},
};

static MSDI MainMsgDispatch = {
    sizeof(MainMsgFunctions) / sizeof(MainMsgFunctions[0]),
    MainMsgFunctions,
    edwpWindow
};

/*
 * Main window command dispatch table definition.
 */

static CMD MainCmdFunctions[] = {
    {ID_FILE_EXIT,							CmdExit},

	{ID_UPDATE_STRETCHDIBITS,				CmdSetBlitType},
	{ID_UPDATE_DIBSECTION,					CmdSetBlitType},
	{ID_UPDATE_WING,						CmdSetBlitType},
	{ID_UPDATE_DUMMY,						CmdSetBlitType},

	{ID_UPDATE_FORCE_ALL,					CmdToggleForceAll},

    {ID_WORLD_UPDATE_NEVER,		  			CmdSetWorldUpdate},
    {ID_WORLD_UPDATE_ALWAYS,	 			CmdSetWorldUpdate},
    {ID_WORLD_UPDATE_FOREGROUND,			CmdSetWorldUpdate},

    {ID_HELP_ABOUT,							CmdAbout},
};

static CMDI MainCmdDispatch = {
    sizeof(MainCmdFunctions) / sizeof(MainCmdFunctions[0]),
    MainCmdFunctions,
    edwpWindow
};

/*
 * MainWindowRegister()
 *
 * Attempt to register a window class for simple view window
 */
BOOL MainWindowRegister(void)
{
    WNDCLASS  wc;

	/*
	 * Build window class structure
	 */
	memset(&wc,0,sizeof(wc));

    wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = (WNDPROC)WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = sizeof(brwin_view *); /* Instance has ptr to view */
    wc.hInstance     = AppInstance;
    wc.hIcon         = LoadIcon (AppInstance, AppName);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszMenuName  = AppName;
    wc.lpszClassName = AppName;

    /*
     * Register the window class and return FALSE if unsuccesful.
	 */
    if (!RegisterClass(&wc))
        return FALSE;

	/* XXX
	 * Could put pointer to the message & command dispatch tables
	 * in extra data and make the WndProc be generic
	 */
    return TRUE;
}

/*
 * WndProc()
 *
 * Windows procedure that calls standard dispatch mechanism
 */
static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispMessage(&MainMsgDispatch, hwnd, uMessage, wparam, lparam);
}

/**
 ** Message handling functions
 **/

/*
 * MsgCommand()
 *
 * Handles WM_COMMAND by invoking standard dispatcher
 */
static LRESULT MsgCommand(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
    return DispCommand(&MainCmdDispatch, hwnd, wparam, lparam);
}

/*
 * MsgCreate()
 *
 * Handles WM_CREATE messages
 * 
 * Creates any sub-windows and puts the menu checks/enable into
 * the initial state
 */
static LRESULT MsgCreate(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	int i;
	HMENU menu = GetMenu(hwnd);
 	LPCREATESTRUCT 	cs = (LPCREATESTRUCT)lparam;
	brwin_view *view;

	/*
 	 * Application view pointer is passed through
	 * the CREATESTRUCT
	 */
	view = (brwin_view *)cs->lpCreateParams;

	/*
	 * Store the view pointer in window extra data
	 */
	INST_VIEW_SET(hwnd,view);

	/*
	 * Set the initial menu states
	 */
	CheckMenuItem(menu, ID_UPDATE+view->blit_type, MF_CHECKED);
	CheckMenuItem(menu, ID_WORLD_UPDATE+view->world->update ,MF_CHECKED);

	for(i=0; i < BUFFER_COUNT; i++)
		EnableMenuItem(menu, ID_UPDATE+i, BufferClasses[i].available?MF_ENABLED:MF_GRAYED);

	CheckMenuItem(menu, ID_UPDATE_FORCE_ALL, view->force_all?MF_CHECKED:MF_UNCHECKED);

	/*
	 * Paint window 
	 */
	UpdateWindow(hwnd);

    return 0;
}

/*
 * MsgDestroy()
 *
 * Handles WM_DESTROY messages
 */
static LRESULT MsgDestroy(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	brwin_view *view = INST_VIEW_GET(hwnd);

	/*
	 * Tear down view structure if still attached
	 */
	if(view) {
		view->window = NULL;
		WindowViewFree(view);
	}

	/*
	 * Single window application - exiting window quits app.
	 */
    PostQuitMessage(0);
    return 0;
}

/*
 * MsgMouseMove()
 *
 * Handles WM_MOUSEMOVE messages
 *
 */
static LRESULT MsgMouseMove(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	/*
	 * Remember move
	 */
	return TRUE;
}

/*
 * MsgMouseDown()
 *
 * Handles WM_?BUTTONDOWN messages
 */
static LRESULT MsgMouseDown(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	/*
	 * Change interaction state
	 */
	return TRUE;
}

/*
 * MsgMouseUp()
 *
 * Handles WM_?BUTTONUP messages
 */
static LRESULT MsgMouseUp(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	/*
	 * Change interaction state
	 */
	return TRUE;
}

/*
 * MsgSize()
 *
 * Handles WM_SIZE messages
 */
static LRESULT MsgSize(HWND hwnd, UINT uMessage, WPARAM wparam, LPARAM lparam)
{
	brwin_view *view = INST_VIEW_GET(hwnd);

	/*
	 * Rebuild the off screen buffer
	 */
	ViewBufferSet(view,view->blit_type, 0, 0, LOWORD(lparam), HIWORD(lparam));

	/*
	 * Redraw world
	 */
	ViewRender(view);

	/*
	 * Update the palette - otherwise we get one
	 * frame of the default palette on startup
	 */
    SelectPalette(view->screen, view->palette, FALSE);
    RealizePalette(view->screen);

	ViewTitleBarUpdate(view);

	return 0;
}

/* 
 * MsgActivateApp()
 *
 * Handles WM_ACTIVEATEAPP messages
 */
static LRESULT MsgActivateApp(HWND hwnd, UINT uMessage, WPARAM fActive, LPARAM dwThreadID)	
{
	/*
	 * Record current Foreground/Background state in a global variable
	 */
	AppIsForeground = fActive;

	return 0;
}

/*
 * MsgPaint()
 *
 * Handles WM_PAINT messages
 */
static LRESULT MsgPaint(HWND hwnd, UINT uMessage, WPARAM wParam, LPARAM lParam)	
{
	brwin_view *view = INST_VIEW_GET(hwnd);
	PAINTSTRUCT ps;
	HDC hdc;

	/*
	 * Blit whole screen thru paint DC
	 */
	hdc = BeginPaint(hwnd,&ps);
    ViewScreenUpdate(view, hdc, 1);
	EndPaint(hwnd,&ps);

	return 0;
}

/*
 * MsgPalette()
 *
 * Handle both WM_QUERYNEWPALETTE and WM_PALETTECHANGED
 *
 */
static LRESULT MsgPalette(HWND hwnd, UINT uMessage, WPARAM hwndPalChg, LPARAM lParam)	
{
	brwin_view *view = INST_VIEW_GET(hwnd);

	/*
	 * Don't do anything if this is a PALETTECHANGED message generated
	 * by ourselves  - we have already had the QUERYNEWPALETTE
	 */
	if(uMessage == WM_PALETTECHANGED && hwnd == (HWND)hwndPalChg)
		return 1L;

    SelectPalette(view->screen, view->palette, FALSE);
    RealizePalette(view->screen);

	/*
	 * Repaint window through new palette
	 */
    InvalidateRect(hwnd,NULL,FALSE);

	return 0;
}

/**
 ** Command handling functions
 **/

/*
 * CmdExit()
 *
 * Handles ID_FILE_EXIT commands
 */
static LRESULT CmdExit(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	/*
	 * Close the current window - which will in turn clear up the view
	 */
    DestroyWindow(hwnd);
    return 0;
}


/*
 * CmdSetBlitType()
 *
 * Handles the ID_UPDATE_* commands that control the various ways
 * that the off-screen buffer can be blitted onto the screen
 */
static LRESULT CmdSetBlitType(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	brwin_view *view = INST_VIEW_GET(hwnd);
	int old_blit_type = view->blit_type;
	int new_blit_type = wCommand - ID_UPDATE;
	HMENU hmenu = GetMenu(hwnd);

	assert(new_blit_type >= 0);
	assert(new_blit_type < BUFFER_COUNT);

	/*
	 * Uncheck any old item
	 */
	CheckMenuItem(hmenu, ID_UPDATE+view->blit_type, MF_UNCHECKED);
	
	/*
	 * Remember the new type
	 */
	CheckMenuItem(hmenu, wCommand, MF_CHECKED);

	/*
	 * Recreate the off-screen buffer according to the new
	 * format
	 */
	if(old_blit_type != new_blit_type) {
		/*
		 * Build new buffer
		 */
		ViewBufferSet(view, new_blit_type, view->x, view->y, view->width, view->height);

		/*
		 * Render the scene
		 */
		ViewRender(view);

		/*
		 * Repaint
		 */
		InvalidateRect(hwnd,NULL,FALSE);
	}

	 return 0;
}

/*
 * CmdSetWorldUpdate()
 *
 * Handles ID_WORLD_UPDATE_* commands. THse control the update of
 * the world database over time.
 *
 */
static LRESULT CmdSetWorldUpdate(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	brwin_view *view = INST_VIEW_GET(hwnd);
	int old_world_update = view->world->update;
	int new_world_update = wCommand - ID_WORLD_UPDATE;
	HMENU hmenu = GetMenu(hwnd);

	assert(new_world_update >= 0);
	assert(new_world_update < WORLD_UPDATE_COUNT);

	/*
	 * Uncheck any old item
	 */
	CheckMenuItem(hmenu, ID_WORLD_UPDATE+old_world_update, MF_UNCHECKED);
	
	/*
	 * Remember the new type
	 */
	view->world->update = new_world_update;
	CheckMenuItem(hmenu, wCommand, MF_CHECKED);

	return 0;
}

/*
 * CmdToggleForceAll()
 *
 * Handles ID_UPDATE_FORCE_ALL command. This toggles the force_all flag
 * in the associcated view. If the force_all flag is set, then the whole
 * off screen buffer will be blitted every frame, rhater than just the
 * changing rectangle.
 */
static LRESULT CmdToggleForceAll(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
	brwin_view *view = INST_VIEW_GET(hwnd);
	HMENU hmenu = GetMenu(hwnd);

	/*
	 * Toggle flag in view
	 */
	view->force_all = !view->force_all;

	/*
	 * Update checkmark
	 */
	CheckMenuItem(hmenu, ID_UPDATE_FORCE_ALL,
		view->force_all?MF_CHECKED:MF_UNCHECKED);
	
	return 0;
}

/*
 * CmdAbout()
 *
 * Handles ID_HELP_ABOUT command. Pops up a rather boring 'About' box.
 */
static LRESULT CmdAbout(HWND hwnd, WORD wCommand, WORD wNotify, HWND hwndCtrl)
{
    DialogBox(AppInstance, "ABOUT_BOX", hwnd, (DLGPROC)AboutDlgProc);
    return 0;
}

