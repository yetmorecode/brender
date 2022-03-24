#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

#include "brender.h"
#include "brwrap.h"

/*
 * Black background
 */
#define BACK_COLOUR 0

#define MAX_DIRTY_RECTANGLES 16

/*
 * Table of dirty rectangles
 */
typedef struct brwin_dirty_rect {
	int min_x;
	int min_y;
	int max_x;
	int max_y;
} brwin_dirty_rect;

brwin_dirty_rect dirtyRectangles[MAX_DIRTY_RECTANGLES];
int lastDirtyRectangle;

int count = 0;

brwin_dirty_rect renderBounds;

/*
 * Forward declarations
 */
static void MinimalBegin(brw_application *app, int argc, char **argv);
static void MinimalEnd(brw_application *app);
static br_pixelmap *MinimalPalette(brw_application *app, int base, int range);
static void MinimalDestination(brw_application *app, br_pixelmap *destination);
static void MinimalRender(brw_application *app, br_bounds2i *bounds);
static int MinimalUpdate(brw_application *app);
static void MinimalEvent(brw_application *app, struct brw_event *event);

void BR_CALLBACK MinimalBoundsCallback(
	br_actor *actor,
	br_model *model,
	br_material *material,
	void *render_data,
	br_uint_8	style,
	br_matrix4 *model_to_screen,
	br_int_32 *bounds);

/*
 * Structure used to describe entry points to application
 */
char *Minimal_args[] = {
	"-test",
	"1",
	"-other",
	"2",
};
static brw_application Minimalapp = {
	"Minimal - DFW",
	"Sam Littlewood",
	"Copyright (c) 1995 by Argonaut Technologies Ltd.",
	0,0,
	512,512,
	BR_ASIZE(Minimal_args),Minimal_args,
	MinimalBegin,
	MinimalEnd,
	MinimalPalette,
	MinimalDestination,
	MinimalRender,
	MinimalUpdate,
	MinimalEvent,
};

/*
 * Public entry pint called by wrapper
 */
brw_application *AppQuery(char *host, int width, int height, int type)
{
	return &Minimalapp;
}

/*
 * Application variables
 */
static br_pixelmap *colour_buffer;
static br_pixelmap *depth_buffer;

static br_actor *root;
static br_actor *camera;
static br_actor *actor;
static br_actor *light;

#define PRIMITIVES 10000
static void MinimalBegin(brw_application *app, int argc, char **argv)
{
	br_material *material;
	void * primitives;
	
	primitives = BrMemAllocate(PRIMITIVES,BR_MEMORY_APPLICATION);

	BrZsBegin(BR_PMT_INDEX_8,primitives,PRIMITIVES);

	material=BrMaterialAllocate("grey_flat");
	material->index_base=10;
	material->index_range=32;
	BrMaterialAdd(material);

 	root = BrActorAllocate(BR_ACTOR_NONE,NULL);
	camera = BrActorAdd(root,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	((br_camera *) camera->type_data)->type = BR_CAMERA_PERSPECTIVE_OLD;

	BrLightEnable(light=BrActorAdd(root,BrActorAllocate(BR_ACTOR_LIGHT,NULL)));

	actor =	BrActorAdd(root,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	actor->material=BrMaterialFind("grey_flat");
	actor->t.type = BR_TRANSFORM_EULER;
	actor->t.t.euler.e.order = BR_EULER_ZXY_S;
	BrVector3Set(&actor->t.t.euler.t,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(-5.0));
}

static void MinimalEnd(brw_application *app)
{
	BrZsEnd();
}

static br_pixelmap *MinimalPalette(brw_application *app, int base, int range)
{
	br_pixelmap *pal;

	pal = BrPixelmapLoad("os2pal.pal");

	return pal;
}

static void MinimalDestination(brw_application *app, br_pixelmap *destination)
{
	/*
	 * Remember new destination
	 */
	colour_buffer = destination;

	/*
	 * Ditch any old depth buffer
	 */
	if(depth_buffer != NULL)
			BrPixelmapFree(depth_buffer);

	/*
	 * Allocate a matching depth buffer
	 */
	depth_buffer = BrPixelmapMatch(colour_buffer, BR_PMMATCH_DEPTH_16);

	/*
	 * Set the camera's aspect ratio
	 */
	((br_camera *)camera->type_data)->aspect = BR_DIV(
			BrIntToScalar(destination->width),
			BrIntToScalar(destination->height));
}

static void MinimalRender(brw_application *app, br_bounds2i *bounds)
{
	br_renderbounds_cbfn *old_cb;

	BrPixelmapFill(colour_buffer,BACK_COLOUR);
	BrPixelmapFill(depth_buffer,0xFFFFFFFF);

	lastDirtyRectangle = 0;
	renderBounds.min_x = INT_MAX;
	renderBounds.min_y = INT_MAX;
	renderBounds.max_x = INT_MIN;
	renderBounds.max_y = INT_MIN;

	old_cb = BrZsSetRenderBoundsCallback(MinimalBoundsCallback);
	BrZsSceneRender(root, camera, colour_buffer);
	BrZsSetRenderBoundsCallback(old_cb);

	bounds->min.v[0] = BrIntToScalar(renderBounds.min_x);
	bounds->min.v[1] = BrIntToScalar(renderBounds.min_y);

	bounds->max.v[0] = BrIntToScalar(renderBounds.max_x);
	bounds->max.v[1] = BrIntToScalar(renderBounds.max_y);

}

static int MinimalUpdate(brw_application *app)
{
static int i=0;
	actor->t.t.euler.e.a = BR_ANGLE_DEG(i);
	actor->t.t.euler.e.b = BR_ANGLE_DEG(i*2);
	actor->t.t.euler.e.c = BR_ANGLE_DEG(i*3);
	i++;
	return 0;
}

char name_buffer[1024];

static void MinimalEvent(brw_application *app, struct brw_event *event)
{
}

/*
 * Callback function used by renderer to pass back dirty rectangles
 */
void BR_CALLBACK MinimalBoundsCallback(
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
	 * Puch rectangle out a bit
	 */
	bounds[BR_BOUNDS_MIN_X] -= 1;
	bounds[BR_BOUNDS_MAX_X] += 1;
	bounds[BR_BOUNDS_MIN_Y] -= 1;
	bounds[BR_BOUNDS_MAX_Y] += 1;

	/*
	 * Accumulate total bounding rectangle
	 */
	if(bounds[BR_BOUNDS_MIN_X] < renderBounds.min_x)
		renderBounds.min_x = bounds[BR_BOUNDS_MIN_X];

	if(bounds[BR_BOUNDS_MIN_Y] < renderBounds.min_y)
		renderBounds.min_y = bounds[BR_BOUNDS_MIN_Y];

	if(bounds[BR_BOUNDS_MAX_X] > renderBounds.max_x)
		renderBounds.max_x = bounds[BR_BOUNDS_MAX_X];

	if(bounds[BR_BOUNDS_MAX_Y] > renderBounds.max_y)
		renderBounds.max_y = bounds[BR_BOUNDS_MAX_Y];

	/*
	 * If list of rectangles is full, merge current into last
	 */

	if(lastDirtyRectangle >= MAX_DIRTY_RECTANGLES) {
		dp = dirtyRectangles + lastDirtyRectangle-1;

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
	dp = dirtyRectangles + lastDirtyRectangle;

	dp->min_x = bounds[BR_BOUNDS_MIN_X];
	dp->min_y = bounds[BR_BOUNDS_MIN_Y];
	dp->max_x = bounds[BR_BOUNDS_MAX_X];
	dp->max_y = bounds[BR_BOUNDS_MAX_Y];

	lastDirtyRectangle++;
}

