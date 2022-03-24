/*
 * world.c
 *
 * (C) Copyright Argonaut Software. 1994.  All rights reserved.
 *
 * Manage a 3D world and the views into it
 */
#include <windows.h>
#include <windowsx.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <brender.h>

#include "dispatch.h"
#include "world.h"
#include "resource.h"
                
#include "buffer.h"

/*
 * Forward declarations
 */
static void SetupTexture(char *material,char *pix_file, br_pixelmap *shade_table);
static void SetupModel(char *name, char *file);
static void BR_CALLBACK ViewBoundsCallback(
	br_actor *actor,
	br_model *model,
	br_material *material,
	void *render_data,
	br_uint_8	style,
	br_matrix4 *model_to_screen,
	br_int_32 *bounds);

/*
 * Keeps a copy of the current view for use by the bounds callback
 */
static brwin_view *bounds_view;

/*
 * Create a new 3D world
 */
brwin_world *WorldAllocate(void)
{
	int nmats;
	brwin_world *world;
	br_camera *camera_data;
	br_light *light_data;
	br_actor *a;
	br_material *mats[50];

	/*
	 * Allocate world structure
	 */
	world = BrMemAllocate(sizeof(*world),BR_MEMORY_APPLICATION);
	world->update = WORLD_UPDATE_FOREGROUND;

	/*
	 * Load some materials
	 */
	BrTableFindHook(BrTableFindFailedLoad);
	BrMapFindHook(BrMapFindFailedLoad);

	nmats = BrFmtScriptMaterialLoadMany("simpview.mat",mats,BR_ASIZE(mats));
	BrMaterialAddMany(mats,nmats);

	/*
	 * Load all the models
	 */
	SetupModel("teapot","teapot.dat");
	SetupModel("cube","cube2.dat");

	/*
	 * Create the world, with default material
	 */
	world->root = BrActorAllocate(BR_ACTOR_NONE,NULL);

	/*
	 * Camera
	 */
	world->camera = BrActorAdd(world->root,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	camera_data = world->camera->type_data;

	camera_data->type = BR_CAMERA_PERSPECTIVE_OLD;
	camera_data->field_of_view = BR_ANGLE_DEG(45.0);
	camera_data->hither_z = BR_SCALAR(0.2);
	camera_data->yon_z = BR_SCALAR(40.0);
	camera_data->aspect = BR_SCALAR(1.8);

	BrMatrix34Translate(&world->camera->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(3.5));

	/*
	 * Light
	 */
	world->light = BrActorAdd(world->root,BrActorAllocate(BR_ACTOR_LIGHT,NULL));
	light_data = world->light->type_data;
	light_data->type = BR_LIGHT_DIRECT;
	light_data->attenuation_c = BR_SCALAR(1.0);

	BrMatrix34RotateY(&world->light->t.t.mat,BR_ANGLE_DEG(45.0));
	BrMatrix34PostRotateZ(&world->light->t.t.mat,BR_ANGLE_DEG(45.0));

	BrLightEnable(world->light);

	/*
	 * Test shape and orbiting cube
	 */
	world->actor = BrActorAdd(world->root,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	world->actor->model = BrModelFind("teapot");
	world->actor->material = BrMaterialFind("test_texture");
	world->actor->t.type = 

	world->actor->t.type = BR_TRANSFORM_EULER;
	world->actor->t.t.euler.e.order = BR_EULER_ZXY_S;
	world->actor->t.t.euler.e.b = BR_ANGLE_DEG(-90);
	BrVector3Set(&world->actor->t.t.euler.t,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(0.0));

	/*
	 * ... child cube
	 */
	a = BrActorAdd(world->actor,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	a->model = BrModelFind("cube");
	a->material = BrMaterialFind("blue");

	BrMatrix34Translate(&a->t.t.mat,BR_SCALAR(1.0),BR_SCALAR(0.0),BR_SCALAR(0.0));
	BrMatrix34PreScale(&a->t.t.mat,BR_SCALAR(0.5),BR_SCALAR(0.5),BR_SCALAR(0.5));

	BrZbSetRenderBoundsCallback(ViewBoundsCallback);

	return world;
}	  


/*
 * Load a model, set it's name, and add it to the renderer
 */
static void SetupModel(char *name, char *file)
 {
 	br_model *m;

    m = BrModelLoad(file);
	if (m == NULL)
		BR_ERROR1("SetupModel: could not load model '%s'",file);

	if(m->identifier)
		BrResFree(m->identifier);

	m->identifier = BrResStrDup(m,name);

	BrModelAdd(m);
}


/*
 * Release the 3D world
 */
void WorldFree(brwin_world *world)
{
	/* XXX Relies on memeory allocator to release everything */
}

/*
 * Time based update of the 3D world
 */
void WorldUpdate(brwin_world *world)
{
	world->actor->t.t.euler.e.a += BR_ANGLE_DEG(10);
}

/*
 * Setup the view buffer
 */
void ViewBufferSet(brwin_view *view, int type, int x, int y, int width, int height)
{
	/*
	 * Store new values
	 */
	view->blit_type = type;
	view->x = x;
	view->x = y;
	view->width = width;
	view->height = height;

	/*
	 * Release all bitmaps
	 */
 	if(view->buffer) {
		view->buffer->free(view->buffer);
		view->buffer = NULL;
	}

	if(view->colour_buffer)
		BrPixelmapFree(view->colour_buffer);

	if(view->depth_buffer)
		BrPixelmapFree(view->depth_buffer);

	/*
	 * Reallocate new maps
	 */
//	ASSERT(type < BUFFER_COUNT);

	view->buffer = BufferClasses[view->blit_type].allocate(view->screen, view->palette,
		view->width, view->height);

	if(view->buffer == NULL)
		return;
	view->colour_buffer = BrPixelmapAllocate(BR_PMT_INDEX_8,
			(br_uint_16)width,(br_uint_16)height,
			view->buffer->bits, BR_PMAF_INVERTED);

	view->depth_buffer = BrPixelmapMatch(
			view->colour_buffer, BR_PMMATCH_DEPTH_16);

	/*
	 * Remeber camera aspect ratios
	 */
	 view->aspect = BrScalarToFixed(BR_DIV(BrIntToScalar(width),BrIntToScalar(height)));

	/*
	 * Clear buffers
	 */
	BrPixelmapFill(view->colour_buffer,0); 
	BrPixelmapFill(view->depth_buffer,0xFFFF);

	view->last_dirty_rectangle = 0;

	/*
	 * Redraw whole buffer on next update
	 */
	view->clear_bounds.min_x = 0;
	view->clear_bounds.min_y = 0;
	view->clear_bounds.max_x = view->width-1;
	view->clear_bounds.max_y = view->height-1;
}

/*
 * Render an image of the world into test_colour_buffer and accumulate a list
 * of changes 
 */
void ViewRender(brwin_view *view)
{
	int i;
	brwin_dirty_rect *dp;

	/*
	 * Don't do anything if off-screen buffer has not been setup yet
	 */
	if(view->buffer == NULL)
		return;

	/*
	 * Clear screen and Z buffer using dirty rectangle list 
	 */
	for(i=0, dp = view->dirty_rectangles ; i< view->last_dirty_rectangle; i++, dp++) {
		BrPixelmapRectangleFill(view->colour_buffer,
			(br_int_16)dp->min_x,(br_int_16)dp->min_y,
			(br_int_16)(dp->max_x-dp->min_x),(br_int_16)(dp->max_y-dp->min_y),
			0);

		BrPixelmapRectangleFill(view->depth_buffer,
			(br_int_16)dp->min_x,(br_int_16)dp->min_y,
			(br_int_16)(dp->max_x-dp->min_x),(br_int_16)(dp->max_y-dp->min_y),
			0xFFFFFFFF);
	}

	/*
	 * Reset dirty list
	 */
	view->last_dirty_rectangle = 0;
	view->render_bounds.min_x = INT_MAX;
	view->render_bounds.min_y = INT_MAX;
	view->render_bounds.max_x = INT_MIN;
	view->render_bounds.max_y = INT_MIN;

	/*
	 * Hook dirty rectangle callback, and render view
	 */
	((br_camera *)view->world->camera->type_data)->aspect = BrFixedToScalar(view->aspect);

	bounds_view = view;

	BrZbSceneRender(view->world->root,
					view->world->camera,
					view->colour_buffer,
					view->depth_buffer);
}

/*
 * Callback function used by renderer to pass back dirty rectangles
 */
static void BR_CALLBACK ViewBoundsCallback(
	br_actor *actor,
	br_model *model,
	br_material *material,
	void *render_data,
	br_uint_8	style,
	br_matrix4 *model_to_screen,
	br_int_32 *bounds)
{
	brwin_dirty_rect *dp;

	/*
	 * Accumulate total bounding rectangle
	 */
	if(bounds[BR_BOUNDS_MIN_X] < bounds_view->render_bounds.min_x)
		bounds_view->render_bounds.min_x = bounds[BR_BOUNDS_MIN_X];

	if(bounds[BR_BOUNDS_MIN_Y] < bounds_view->render_bounds.min_y)
		bounds_view->render_bounds.min_y = bounds[BR_BOUNDS_MIN_Y];

	if(bounds[BR_BOUNDS_MAX_X] > bounds_view->render_bounds.max_x)
		bounds_view->render_bounds.max_x = bounds[BR_BOUNDS_MAX_X];

	if(bounds[BR_BOUNDS_MAX_Y] > bounds_view->render_bounds.max_y)
		bounds_view->render_bounds.max_y = bounds[BR_BOUNDS_MAX_Y];

	/*
	 * If list of rectangles is full, merge current into last
	 */
	if(bounds_view->last_dirty_rectangle >= MAX_DIRTY_RECTANGLES) {
		dp = bounds_view->dirty_rectangles + bounds_view->last_dirty_rectangle-1;

		if(bounds[BR_BOUNDS_MIN_X] < dp->min_x)
			dp->min_x = bounds[BR_BOUNDS_MIN_X];

		if(bounds[BR_BOUNDS_MIN_Y] < dp->min_y)
			dp->min_y = bounds[BR_BOUNDS_MIN_Y];

		if(bounds[BR_BOUNDS_MAX_X] > dp->max_x)
			dp->max_x = bounds[BR_BOUNDS_MAX_X];

		if(bounds[BR_BOUNDS_MAX_Y] > dp->max_y)
			dp->max_y = bounds[BR_BOUNDS_MAX_Y];

		return;
	}

	/*
	 * Add this rectangle to list
	 */
	dp = bounds_view->dirty_rectangles + bounds_view->last_dirty_rectangle;

	dp->min_x = bounds[BR_BOUNDS_MIN_X];
	dp->min_y = bounds[BR_BOUNDS_MIN_Y];
	dp->max_x = bounds[BR_BOUNDS_MAX_X];
	dp->max_y = bounds[BR_BOUNDS_MAX_Y];

	bounds_view->last_dirty_rectangle++;
}

/*
 * Update screen from offscreen bitmap - will attempt to
 * only update the changed area, unless force_all is true
 */
void ViewScreenUpdate(brwin_view *view, HDC screen, int force_all)
{
	int x,y,w,h;

	/*
	 * If no context passed in, use cached one
	 */
	if(screen == NULL)
		screen=view->screen;

	/*
	 * Don't do anything if off-screen buffer has not been setup yet
	 */
	if(view->buffer == NULL)
		return;

	/*
	 * Override force_all flag with any view specific setting
	 */
	force_all |= view->force_all;

	if(force_all || view->render_bounds.min_x > view->render_bounds.max_x) {
		/*
		 * Update the whole client area
		 */
		view->clear_bounds.min_x = x = 0;
		view->clear_bounds.min_y = y = 0;
		view->clear_bounds.max_x = (w = view->width)-1;
		view->clear_bounds.max_y = (h = view->height)-1;
	} else {
		/*
		 * Only copy the union of view->clear_bounds and view->render_bounds
		 */
		x = (view->render_bounds.min_x<view->clear_bounds.min_x) ? view->render_bounds.min_x : view->clear_bounds.min_x;
		y = (view->render_bounds.min_y<view->clear_bounds.min_y) ? view->render_bounds.min_y : view->clear_bounds.min_y;

		w = (view->render_bounds.max_x>view->clear_bounds.max_x) ? view->render_bounds.max_x : view->clear_bounds.max_x;
		h = (view->render_bounds.max_y>view->clear_bounds.max_y) ? view->render_bounds.max_y : view->clear_bounds.max_y;

		w = w + 1 - x;
		h = h + 1 - y;

		view->clear_bounds = view->render_bounds;
	}

	view->buffer->blit(view->buffer, screen, view->x+x,view->y+y, x,y ,w,h);

#if 0
	ViewDrawDirtyRectangles(view, screen);
#endif
}


/*
 * Maintain frame rate counter in title bar of window - should be called once per frame
 *
 * Returns TRUE if a new framerate is available
 */
int ViewFrameRateUpdate(brwin_view *view)
{
	long elapsed_time;

	view->frame_count++;
		
	if(view->frame_count >= FRAMERATE_COUNT) {
		elapsed_time = timeGetTime() - view->start_time;
									  
		view->frame_rate = FRAMERATE_COUNT * 100000 / elapsed_time;
		view->frame_count = 0;

		ViewTitleBarUpdate(view);

		view->start_time = timeGetTime();

		return 1;
	}

	return 0;
}

/*
 * ViewTitleBarUpdate()
 *
 * Build the title text of the window from the title, the width and height, and the frame rate
 */
void ViewTitleBarUpdate(brwin_view *view)
{
 	char title[50];

 	wsprintf(title, "%s - %2d.%02d (%d,%d)", view->title,
 		view->frame_rate / 100, view->frame_rate % 100,
 		view->width ,view->height);

	SetWindowText(view->window, title);
}

/*
 * ViewDrawDirtyRectangles()
 *
 * Draw a rectangle around each dirty rectangle in current list
 */
void ViewDrawDirtyRectangles(brwin_view *view, HDC screen)
{
	HPEN old_pen,red_pen;
	HBRUSH old_brush;
	int i;
	brwin_dirty_rect *dp;
	
	/*
	 * Use cached context if none given
	 */
	if(screen == NULL)
		screen = view->screen;

	red_pen = CreatePen(PS_SOLID,0,RGB(255,0,0));

	old_pen = SelectObject(screen, GetStockObject(WHITE_PEN));
	old_brush = SelectObject(screen, GetStockObject(NULL_BRUSH));

	/*
	 * Individual rectangles
	 */
	for(i=0, dp = view->dirty_rectangles ; i< view->last_dirty_rectangle; i++, dp++)
		Rectangle(screen, dp->min_x, dp->min_y, dp->max_x, dp->max_y);

	/*
	 * Total rectangle
	 */
	SelectObject(screen,red_pen);
	Rectangle(screen,
			view->render_bounds.min_x, view->render_bounds.min_y,
			view->render_bounds.max_x, view->render_bounds.max_y);

	SelectObject(screen,old_pen);
	SelectObject(screen,old_brush);

	DeleteObject(red_pen);
}
