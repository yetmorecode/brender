/*
 * world.h
 *
 * (C) Copyright Argonaut Software. 1995.  All rights reserved.
 *
 * A 3D world and views into it
 */

/*
 * Global constants
 */
#define FRAMERATE_COUNT 20

/*
 * The 3D world 
 */
typedef struct brwin_world {
	/*
	 * World hierachy
	 */
	struct br_actor *root;
	struct br_actor *camera;
	struct br_actor *light;
	struct br_actor *actor;

	/*
	 * What sort of update should be carried out
	 */
	int		update;

} brwin_world;

#define WORLD_UPDATE_ALWAYS		0
#define WORLD_UPDATE_FOREGROUND	1
#define WORLD_UPDATE_NEVER		2
#define WORLD_UPDATE_COUNT		3

#define MAX_DIRTY_RECTANGLES 16

/*
 * A view into the 3D world
 */
typedef struct brwin_dirty_rect {
	int min_x;
	int min_y;
	int max_x;
	int max_y;
} brwin_dirty_rect;

typedef struct brwin_view {
	/*
	 * World into which this is a view
	 */
	brwin_world	*world;	

	HWND	window;		/* Handle to output window		*/
	HDC		screen;		/* Handle to a DC for the window */
	HPALETTE palette;	/* Logical palette				*/
	char	*title;		/* Title string					*/
	
	int		x,y;		/* Position in window of image	*/
	int		width,height;/* Size of image				*/

	long aspect;		/* aspect ratio for camera (fixed point) */

	int		blit_type;	/* How to blit the window		*/
	int		force_all;	/* Always update whole region	*/

	/*
	 * State for working out rendering rate
	 */
	long	start_time;
	int		frame_count;
	int		frame_rate;

	/*
	 * Current off screen buffer
	 */
	struct brwin_buffer *buffer;

	/*
	 * Rendering buffers
	 */
	struct br_pixelmap *colour_buffer;
	struct br_pixelmap *depth_buffer;

	/*
	 * Table of dirty rectangles
	 */
	brwin_dirty_rect dirty_rectangles[MAX_DIRTY_RECTANGLES];
	int last_dirty_rectangle;

	/*
	 * Union of rendered rectangles
	 */
	brwin_dirty_rect render_bounds;

	/*
	 * Rectangle that needs clearing from last frame
	 */
	brwin_dirty_rect clear_bounds;

} brwin_view;

/*
 * World/view function prototypes
 */
brwin_world *WorldAllocate(void);
void WorldFree(brwin_world *world);
void WorldUpdate(brwin_world *world);

void ViewBufferSet(brwin_view *view, int type, int x, int y, int width, int height);
void ViewRender(brwin_view *view);
void ViewScreenUpdate(brwin_view *view, HDC dc, int force_all);

int ViewFrameRateUpdate(brwin_view *view);
void ViewTitleBarUpdate(brwin_view *view);
void ViewDrawDirtyRectangles(brwin_view *view, HDC screen);

