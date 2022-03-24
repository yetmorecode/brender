/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id:  $
 * $Locker:  $
 *
 */
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#include "brender.h"
#include "dosio.h"

/*
 * Structure used to hold information for the
 * pick callbacks
 */
typedef struct br_pick_nearest {
	br_actor *actor;
	br_model *model;
	br_material *material;
	br_vector3 point;
	int face;
	int edge;
	int vertex;
	br_vector2 map;

	br_scalar t;
	br_actor *temp_actor;
} br_pick_nearest;

static br_pick_nearest pickNearest;

/*
 * The screen, offscreen buffer, and the depth buffer
 */
static br_pixelmap *screenBuffer, *backBuffer, *depthBuffer;

/*
 * Global information abount the cursor
 */
br_boolean CursorFlag = BR_FALSE;

br_int_32 CursorX,CursorY;
br_uint_32 CursorColour = 255;

/*
 * Global pointer for picking material
 */
br_material *PickMaterial;

/*
 * Forward declarations
 */
int Control(br_actor *world, br_actor *observer, br_actor *target);
void ControlCursor(br_actor *world, br_actor *observer, br_actor *target);
void ControlObject(br_actor *target);
void Matrix33Mul(br_matrix34 *A, br_matrix34 *B, br_matrix34 *C);
void dprintf(int x, int y, char *fmt,...);

int main(int argc, char **argv)
{
	br_actor *observer, *world;
	br_actor *a;
	br_pixelmap *palette;
	br_camera *camera;
	br_material *mats[100];
	int n;

	/*
	 * Setup renderer and screen
	 */
	BrBegin();

	/*
	 * make tables, maps and models autoload
	 */
    BrTableFindHook(BrTableFindFailedLoad);
    BrMapFindHook(BrMapFindFailedLoad);
    BrModelFindHook(BrModelFindFailedLoad);

	/*
	 * Set up screen
	 */
	screenBuffer = DOSGfxBegin(NULL);
	BrZbBegin(screenBuffer->type,BR_PMT_DEPTH_16);

	/*
	 * Setup CLUT (ignored in true-colour)
	 */
	palette = BrPixelmapLoad("std.pal");
	if(palette)
		DOSGfxPaletteSet(palette);

	/*
	 * Allocate other buffers
	 */
	backBuffer = BrPixelmapMatch(screenBuffer,BR_PMMATCH_OFFSCREEN);
	depthBuffer = BrPixelmapMatch(screenBuffer,BR_PMMATCH_DEPTH_16);

	/*
	 * Load materials from pick.mat
	 */
    n = BrFmtScriptMaterialLoadMany("pick.mat",mats,BR_ASIZE(mats));
    BrMaterialAddMany(mats,n);

	/*
	 * Remember a pointer to a material for testing pick2d
	 */
	PickMaterial = BrMaterialFind("green");

	/*
	 * Setup keyboard & mouse
	 */
	DOSMouseBegin();
	DOSKeyBegin();

	/*
	 * Build the world
	 */
	world = BrActorAllocate(BR_ACTOR_NONE,NULL);
	observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	camera = (br_camera *)(observer->type_data);

	camera->type = BR_CAMERA_PERSPECTIVE_OLD;
	camera->field_of_view = BR_ANGLE_DEG(55.0);
	camera->hither_z = BR_SCALAR(0.01); 
	camera->yon_z = BR_SCALAR(50.0); 
	camera->aspect = BR_SCALAR(1.6); 

	BrMatrix34Translate(&observer->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(3.0));

	BrLightEnable(BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT,NULL)));

	a = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));

	/*
	 * If an argument of given, use that as a model
	 */
	a->model = BrModelLoad((argc >= 2)?argv[1]:"torus.dat");

	if(a->model) {
		a->model->flags |= BR_MODF_UPDATEABLE;
		BrModelAdd(a->model);
	}

	/*
	 * If a material is given, set that
	 */
	if(argc  >= 3)
		a->material = BrMaterialFind(argv[2]);

	/*
	 * Display loop
	 */
	for(;;) {
		/*
		 * Clear the buffers
		 */
		BrPixelmapFill(backBuffer,0);
		BrPixelmapFill(depthBuffer,0xFFFFFFFF);

		/*
		 * Render 3D world
		 */
		BrZbSceneRender(world,observer,backBuffer,depthBuffer);

		/*
		 * Render Cursor if visible
		 */
		if(CursorFlag) {
			int x = CursorX + screenBuffer->width/2;
			int y = CursorY + screenBuffer->height/2;

			BrPixelmapLine(backBuffer,x+2,y,x+6,y,CursorColour);
			BrPixelmapLine(backBuffer,x-2,y,x-6,y,CursorColour);
			BrPixelmapLine(backBuffer,x,y+2,x,y+6,CursorColour);
			BrPixelmapLine(backBuffer,x,y-2,x,y-6,CursorColour);

			if(pickNearest.model) {
				dprintf(0,0,"Actor:    %s",pickNearest.actor->identifier?pickNearest.actor->identifier:"<NULL>");
				dprintf(0,1,"Model:    %s",pickNearest.model->identifier?pickNearest.model->identifier:"<NULL>");
				dprintf(0,2,"Material: %s",pickNearest.material->identifier?pickNearest.material->identifier:"<NULL>");
				dprintf(0,3,"t = %f",BrScalarToFloat(pickNearest.t));
			}
		}

		/*
		 * Update display
		 */
		BrPixelmapDoubleBuffer(screenBuffer,backBuffer);

		/* 
		 * User controls
		 */
		if(Control(world, observer, a))
			break;
	}

	/*
	 * Close down
	 */
	BrPixelmapFree(depthBuffer);
	BrPixelmapFree(backBuffer);

	DOSKeyEnd();
	DOSMouseEnd();
	BrZbEnd();
	DOSGfxEnd();
	BrEnd();

	return 0;
}

#define MSCALE BR_SCALAR(0.006)

/*
 * Read mouse and keyboard for control info.
 *
 * Return true if user wants to quit
 */
int Control(br_actor *world, br_actor *observer, br_actor *target)
{
	if(CursorFlag)
		ControlCursor(world, observer, target);
	else
		ControlObject(target);
	
	if(DOSKeyTest(SC_Q,0,REPT_FIRST_DOWN))
		return 1;

	if(DOSKeyTest(SC_SPACE,0,REPT_FIRST_DOWN))
		CursorFlag = !CursorFlag;

	return 0;
}

/*
 * Read the mouse to control an actor's translation and rotation
 */
void ControlObject(br_actor *target)
{
	static br_int_32 mouse_x,mouse_y;
	br_uint_32 mouse_buttons;
	br_matrix34 mat_target, mat_roll;
	br_scalar tx,ty,tz;

	DOSMouseRead(&mouse_x,&mouse_y,&mouse_buttons);

	/*
	 * Mouse controls
	 */
	if(mouse_buttons & BR_MSM_BUTTONL) {

		/*
		 * Drag object around
		 */
		tx =  BR_MUL(BR_SCALAR(mouse_x),MSCALE);
		ty = -BR_MUL(BR_SCALAR(mouse_y),MSCALE);

		BrMatrix34PostTranslate(&target->t.t.mat,tx,ty,BR_SCALAR(0.0));

	} else if(mouse_buttons & BR_MSM_BUTTONR) {

		/*
		 * Drag object around
		 */

		tx = BR_MUL(BR_SCALAR(mouse_x),MSCALE);
		tz = BR_MUL(BR_SCALAR(mouse_y),MSCALE);

		BrMatrix34PostTranslate(&target->t.t.mat,tx,BR_SCALAR(0.0),tz);
		
	} else {
		/*
		 * Rotate object via rolling ball interface
		 */
		BrMatrix34RollingBall(&mat_roll,mouse_x,-mouse_y,1000);

		/*
		 * Only modify the top 3x3
		 */
		BrMatrix34Copy(&mat_target, &target->t.t.mat);
		Matrix33Mul(&target->t.t.mat,&mat_target, &mat_roll);
	}

	mouse_x = 0;
	mouse_y = 0;
}

int BR_CALLBACK BrPickNearestModelCallback(
		br_model *model,
		br_material *material,
		br_vector3 *ray_pos, br_vector3 *ray_dir,
		br_scalar t,
		int f,
		int e,
		int v,
		br_vector3 *p,
		br_vector2 *map,
		br_pick_nearest *pn)
{
	if(t < pn->t) {
		pn->t = t;
		pn->actor = pn->temp_actor;
		pn->model = model;
		pn->material = material;
		pn->point = *p;
		pn->face = f;
		pn->edge = e;
		pn->vertex = v;
		pn->map = *map;
	}

	return 0;
}

/*
 * Test callback
 */
int BR_CALLBACK BrPickNearestCallback(
		br_actor *actor,
		br_model *model,
		br_material *material,
		br_vector3 *ray_pos, br_vector3 *ray_dir,
		br_scalar t_near, br_scalar t_far,
		br_pick_nearest *pn)
{
	pn->temp_actor = actor;

	BrModelPick2D(model, material, ray_pos, ray_dir, t_near, t_far,
		(br_modelpick2d_cbfn *)BrPickNearestModelCallback, pn);

	return 0;
}

/*
 * Read mouse to control cursor position
 */
void ControlCursor(br_actor *world, br_actor *observer, br_actor *target)
{
	br_uint_32 mouse_buttons;

	DOSMouseRead(&CursorX,&CursorY,&mouse_buttons);

	if(mouse_buttons) {
		/*
		 * Test pick
		 */
		pickNearest.model = NULL;
		pickNearest.t = BR_SCALAR_MAX;

		BrScenePick2D(world,observer,
			backBuffer,CursorX,CursorY,
			(br_pick2d_cbfn *)BrPickNearestCallback,&pickNearest);


		if(pickNearest.model) {
#if 1
			if(mouse_buttons & BR_MSM_BUTTONL) {
				pickNearest.model->faces[pickNearest.face].material = PickMaterial;
				BrModelUpdate(pickNearest.model,BR_MODU_ALL);
			}
#endif

#if 0
			if(mouse_buttons & BR_MSM_BUTTONL) {
				pickNearest.actor->render_style = BR_RSTYLE_EDGES;
			}
#endif

#if 1
			/*
			 * Scribble in the texture
			 */
			if(mouse_buttons & BR_MSM_BUTTONR) {
				if(pickNearest.material && pickNearest.material->colour_map) {
					br_vector2 t;
					int u,v;

					/*
					 * Push point through map_transform
					 */
					BrMatrix23ApplyP(&t,&pickNearest.map,&pickNearest.material->map_transform);
					t.v[0] = t.v[0] & 0xffff;
					t.v[1] = t.v[1] & 0xffff;
					u = BrScalarToInt(BR_MUL(t.v[0],BrIntToScalar(pickNearest.material->colour_map->width)));
					v = BrScalarToInt(BR_MUL(t.v[1],BrIntToScalar(pickNearest.material->colour_map->height)));

					/*
					 * Set the pixel (texture mappper ignores origin)
					 */
					BrPixelmapPlot(pickNearest.material->colour_map,
							u-pickNearest.material->colour_map->origin_x,
							v-pickNearest.material->colour_map->origin_y,184);
				}
			}
#endif

#if 0
			if(mouse_buttons & BR_MSM_BUTTONR) {
				pickNearest.model->faces[pickNearest.face].material = NULL;
				BrModelUpdate(pickNearest.model,BR_MODU_ALL);
			}
#endif
		}
	}
}


/*
 * Matrix multiply for top 3x3 only
 */
#define A(x,y) A->m[x][y]
#define B(x,y) B->m[x][y]
#define C(x,y) C->m[x][y]

void Matrix33Mul(br_matrix34 *A, br_matrix34 *B, br_matrix34 *C)
{
	A(0,0) = BR_MAC3(B(0,0),C(0,0), B(0,1),C(1,0), B(0,2),C(2,0));
	A(0,1) = BR_MAC3(B(0,0),C(0,1), B(0,1),C(1,1), B(0,2),C(2,1));
	A(0,2) = BR_MAC3(B(0,0),C(0,2), B(0,1),C(1,2), B(0,2),C(2,2));

	A(1,0) = BR_MAC3(B(1,0),C(0,0), B(1,1),C(1,0), B(1,2),C(2,0));
	A(1,1) = BR_MAC3(B(1,0),C(0,1), B(1,1),C(1,1), B(1,2),C(2,1));
	A(1,2) = BR_MAC3(B(1,0),C(0,2), B(1,1),C(1,2), B(1,2),C(2,2));

	A(2,0) = BR_MAC3(B(2,0),C(0,0), B(2,1),C(1,0), B(2,2),C(2,0));
	A(2,1) = BR_MAC3(B(2,0),C(0,1), B(2,1),C(1,1), B(2,2),C(2,1));
	A(2,2) = BR_MAC3(B(2,0),C(0,2), B(2,1),C(1,2), B(2,2),C(2,2));
}


void dprintf(int x, int y, char *fmt,...)
{
	char temp[256];
 	va_list args;

	/*
	 * Build output string
	 */
	va_start(args,fmt);
	vsprintf(temp,fmt,args);
	va_end(args);
	
	BrPixelmapText(backBuffer, x * 4, y * 6, 255, BrFontFixed3x5,  temp);
}


