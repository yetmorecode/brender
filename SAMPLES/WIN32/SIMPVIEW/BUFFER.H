/*
 * buffer.h
 *
 * (C) Copyright Argonaut Software. 1995.  All rights reserved.
 *
 * Managing an off screen buffer and copying it to the screen
 */

/*
 * Generic structure describing a buffer
 */
typedef struct brwin_buffer {
	void *bits;
	int	width;
	int	height;
	void (*blit)(struct brwin_buffer *buffer, HDC screen, int dest_x, int dest_y,
		int	src_x,	int src_y,int width, int height);
	void (*free)(struct brwin_buffer *buffer);
} brwin_buffer;

typedef struct brwin_buffer_class {
	int (*init)(void);
	brwin_buffer *(*allocate)(HDC screen, HPALETTE palette, int width, int height);
	int	available;
} brwin_buffer_class;

/*
 * Available buffering types - indexes into BufferClasses array
 */
#define BUFFER_WING				0
#define BUFFER_STRETCHDIBITS	1
#define BUFFER_DIBSECTION		2
#define BUFFER_DUMMY			3

/*
 * Number of buffering types
 */
#define BUFFER_COUNT		4

/*
 * Table of buffer types
 */
extern brwin_buffer_class BufferClasses[];

/*
 * Prototypes
 */
int BufferClassesInit(void);
