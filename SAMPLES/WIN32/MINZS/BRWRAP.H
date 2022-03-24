/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: brwrap.h 1.1 1995/06/30 16:13:30 sam Exp $
 * $Locker:  $
 *
 * A framework used to host brender demos+applications on several platforms
 */

enum brw_event_type {
	BRW_EVENT_KEY_DOWN,
	BRW_EVENT_KEY_UP,
	/* value_1 == key code		*/

	BRW_EVENT_POINTER1_DOWN,
	BRW_EVENT_POINTER1_UP,
	BRW_EVENT_POINTER2_DOWN,
	BRW_EVENT_POINTER2_UP,
	BRW_EVENT_POINTER3_DOWN,
	BRW_EVENT_POINTER3_UP,
	/* value_1 == x				*/
	/* value_2 == y				*/

	BRW_EVENT_POINTER_MOVE,
	/* value_1 == x				*/
	/* value_2 == y				*/

	BRW_EVENT_TIMER,
	/* value_1 == id			*/
	/* value_2 == shot			*/

	BRW_EVENT_COMMAND,
	/* value_1 == command		*/

	BRW_EVENT_CHAR,
	/* value_1 == ASCII			*/

	BRW_EVENT_MAX
};

/*
 * The mimnimum set of codes that a framework implmenetation
 * will generate are -
 *
 *	BRW_KEY_UP
 *	BRW_KEY_DOWN
 *	BRW_KEY_LEFT
 *	BRW_KEY_RIGHT
 *
 *	BRW_KEY_ENTER
 *	BRW_KEY_ESCAPE
 *
 *	BRW_KEY_PREV
 *	BRW_KEY_NEXT
 *
 *	BRW_KEY_F1
 *	BRW_KEY_F2
 *	BRW_KEY_F3
 *	BRW_KEY_F4
 */

enum brw_keycode {
	BRW_KEY_NONE,

	BRW_KEY_SHIFT,
	BRW_KEY_CONTROL,
	BRW_KEY_ALT,

	BRW_KEY_TAB = '\t',
	BRW_KEY_BACKSPACE = '\0x9',

	BRW_KEY_CANCEL,
	BRW_KEY_SELECT,

	BRW_KEY_UP,
	BRW_KEY_DOWN,
	BRW_KEY_LEFT,
	BRW_KEY_RIGHT,

	BRW_KEY_FIRST,
	BRW_KEY_LAST,

	BRW_KEY_PREV,
	BRW_KEY_NEXT,

	BRW_KEY_SPACE = ' ',

	BRW_KEY_0 = '0',
	BRW_KEY_1,
	BRW_KEY_2,
	BRW_KEY_3,
	BRW_KEY_4,
	BRW_KEY_5,
	BRW_KEY_6,
	BRW_KEY_7,
	BRW_KEY_8,
	BRW_KEY_9,

	BRW_KEY_A = 'A',
	BRW_KEY_B,
	BRW_KEY_C,
	BRW_KEY_D,
	BRW_KEY_E,
	BRW_KEY_F,
	BRW_KEY_G,
	BRW_KEY_H,
	BRW_KEY_I,
	BRW_KEY_J,
	BRW_KEY_K,
	BRW_KEY_L,
	BRW_KEY_M,
	BRW_KEY_N,
	BRW_KEY_O,
	BRW_KEY_P,
	BRW_KEY_Q,
	BRW_KEY_R,
	BRW_KEY_S,
	BRW_KEY_T,
	BRW_KEY_U,
	BRW_KEY_V,
	BRW_KEY_W,
	BRW_KEY_X,
	BRW_KEY_Y,
	BRW_KEY_Z,


	BRW_KEY_F1 = 128,
	BRW_KEY_F2,
	BRW_KEY_F3,
	BRW_KEY_F4,
	BRW_KEY_F5,
	BRW_KEY_F6,
	BRW_KEY_F7,
	BRW_KEY_F8,
	BRW_KEY_F9,
	BRW_KEY_F10,

	BRW_KEY_MAX
};

enum brw_qualifier {
	BRW_QUAL_SHIFT		= 0x01,
	BRW_QUAL_CONTROL	= 0x02,
	BRW_QUAL_ALT		= 0x04,
	BRW_QUAL_POINTER_1	= 0x10,
	BRW_QUAL_POINTER_2	= 0x20,
	BRW_QUAL_POINTER_3	= 0x40
};

typedef struct brw_event {
	br_uint_16 type;
	br_uint_16 qualifiers;
	br_uint_32 value_1;
	br_uint_32 value_2;
} brw_event;


typedef struct brw_application {

	/* Title of application
	 */
	char *title;

	/* Author
	 */
	char *author;

	/* Copyright string
	 */
	char *copyright;

	/* Desired position and size of window, if
	 * appropriate (may be overridden by wrapper)
	 */
	int	x,y;
	int width,height;

	/* Arguments added to command line
	 */
	int argc;
	char **argv;

	/* Create/Teardown world
	 */
	void (*begin)(struct brw_application *wrapper, int argc, char **argv);
	void (*end)(struct brw_application *wrapper);

	/* Inquire if app. would like to set any CLUT entries
	 */
	br_pixelmap *(*palette)(struct brw_application *wrapper, int base, int range);

	/* Hand over a ptr. to the destination frame buffer - invoked on any
	 * change of size or type.
	 */
	void (*destination)(struct brw_application *wrapper, br_pixelmap *screen);

	/* Generate image of world
	 */
	void (*render)(struct brw_application *wrapper, br_bounds2i *bounds);

	/* Update (return true if no more udpates until next event
	 */
	int (*update)(struct brw_application *wrapper);

	/* Events from user
	 */
	void (*event)(struct brw_application *wrapper, struct brw_event *event);

	/*
	 * User pointer
	 */
	void *user;

} brw_application;

/*
 * Value for x,y,w,h to indicate that the
 * the wrapper should use some default
 */
#define BRW_SHAPE_DEFAULT	-1

/*
 * Function provided by application that returns a pointer
 * to the above structure
 */
brw_application *AppQuery(char *host, int width, int height, int type);

/**
 ** Functions provided by wrapper 
 **/

/*
 * Exit
 */
void BrwQuitRequest(void);

/*
 * Event
 */
void BR_PUBLIC_ENTRY BrwPostEvent(brw_event *e);

/*
 * Menus
 */
enum brw_menu_type {
	BRW_MENU_END,
	BRW_MENU_COMMAND,
	BRW_MENU_SEPARATOR,
	BRW_MENU_SUBMENU,
	BRW_MENU_WRAPPER
};

#define BRW_MENUF_CHECK		0x01
#define BRW_MENUF_DISABLE	0x02

typedef struct brw_menu {
	char	type;
	char	flags;
	char	*text;
	int		command;
	void	*data;
} brw_menu;

enum brw_menu_action {
	BRW_ACTION_NOP,
	BRW_ACTION_CLEAR,
	BRW_ACTION_SET,
	BRW_ACTION_TOGGLE
};

void BrwMenuBegin(brw_menu *menus);
int BrwMenuDisable(int item,int action);
int BrwMenuCheck(int item,int action);
void BrwMenuEnd(void);

/*
 * Pointer
 */
enum brw_pointer_type {
	BRW_POINTER_NONE,
	BRW_POINTER_ARROW,
	BRW_POINTER_WAIT,
	BRW_POINTER_CROSS,
	BRW_POINTER_SQUARE,

	BRW_POINTER_MAX
};

void BrwPointerBegin(void);
void BrwPointerPositionGet(int *xp, int *yp);
void BrwPointerPositionSet(int x, int y);
void BrwPointerShapeSet(int pointer_shape);
int BrwPointerShapeGet(void);
void BrwPointerEnd(void);

/*
 * Requesters
 */

enum brw_requestfile_type {
	BRW_FRQ_ANY,
	BRW_FRQ_EXISTS,
	BRW_FRQ_READ,
	BRW_FRQ_WRITE,
	BRW_FRQ_READWRITE,
	BRW_FRQ_MANY 		= 0x80,
};

typedef struct brw_requestfile {
	char *title;
	int type;
	char *extension;
	int buffer_size;
	char *buffer;
	int namec;
	char **namev;
} brw_requestfile;

enum brw_requestlist_type {
	BRW_LRQ_SINGLE,
	BRW_LRQ_MULTIPLE,
};

typedef struct brw_listentry {
	char *text;
	int value;
	char select_in;
	char select_out;
} brw_listentry;

typedef struct brw_requestlist {
	char *title;
	int type;
	int nentries;
	brw_listentry *entries;
} brw_requestlist;

typedef struct brw_requeststring {
	char *title;

	int buffer_size;
	char *buffer;

	char *button1;
	char *button2;
	char *button3;
} brw_requeststring;

typedef struct brw_requestmessage {
	char *title;

	char *message;

	char *button1;
	char *button2;
	char *button3;
} brw_requestmessage;

int BrwRequestFile(brw_requestfile *frq);
int BrwRequestList(brw_requestlist *lrq);
int BrwRequestString(brw_requeststring *srq);
int BrwRequestMessage(brw_requestmessage *mrq);

/*
 * Timer
 */
void BrwTimerStart(int period, int shots, int id);
void BrwTimerStop(int id);

/*
 * Handlers
 */
br_errorhandler *BrwErrorHandler(void);
br_filesystem *BrwFilesystem(void);
br_allocator *BrwAllocator(void);

/*
 * Utility functions
 */
char *BrwEventText(char *dest, brw_event *e);

