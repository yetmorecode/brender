/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id:  $
 * $Locker:  $
 *
 * Example of useing BrModelUpdate() to implement morphing
 */
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#include "brender.h"
#include "dosio.h"

/*
 * Storage for target model sequence
 */
static int numMorphModels;
static br_model *morphModels[50];

br_boolean runMorph = BR_TRUE;

/*
 * The screen, offscreen buffer, and the depth buffer
 */
static br_pixelmap *screenBuffer, *backBuffer, *depthBuffer;

/*
 * Forward declarations
 */
int Control(br_actor *target);
void ControlObject(br_actor *target);
void Matrix33Mul(br_matrix34 *A, br_matrix34 *B, br_matrix34 *C);
void dprintf(int x, int y, char *fmt,...);

void MorphModels(br_model *dest, br_model *model_a, br_model *model_b, br_scalar alpha);
void MorphModelsMany(br_model *dest, br_model **models, int nmodels, br_scalar alpha);

int main(int argc, char **argv)
{
	br_actor *observer, *world;
	br_actor *a;
	br_pixelmap *palette;
	br_camera *camera;
	br_material *mats[100];
	int n;
	char *cp;
	br_scalar alpha;
	br_float time;
	br_boolean running;

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
	 * Load materials from morph.mat
	 */
    n = BrFmtScriptMaterialLoadMany("morph.mat",mats,BR_ASIZE(mats));
    BrMaterialAddMany(mats,n);

	/*
	 * Setup keyboard & mouse
	 */
	DOSMouseBegin();
	DOSKeyBegin();
	DOSClockBegin();

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
	cp = (argc >= 2)?argv[1]:"cubemelt.dat";

	a->model = BrModelLoad(cp);
	/*
	 * Set flags to indicate we want to updat the model
	 */
	a->model->flags |= BR_MODF_UPDATEABLE;

	if(a->model)
		BrModelAdd(a->model);

	/*
	 * Load, BUT DONT ADD, the source models
	 */
	numMorphModels = BrModelLoadMany(cp, morphModels, BR_ASIZE(morphModels));

	if(numMorphModels < 2)
		BR_ERROR1("Could not load models from %s\n",cp);
	/*
	 * If a material is given, set that
	 */
	if(argc  >= 3)
		a->material = BrMaterialFind(argv[2]);
	else
		a->material = BrMaterialFind("environment");

	/*
	 * Display loop
	 */
	for(;;) {

		/*
		 * Test multiple morph
		 */
		if(runMorph) {
			time = DOSClockRead() / (float)BR_DOS_CLOCK_RATE;
			time = time / 2;
			time = fmod(time , 2);

			if(time > 1.0)
				time = 2.0 - time;

			alpha = BrFloatToScalar(time);

			MorphModelsMany(a->model,morphModels,numMorphModels,alpha);
			BrModelUpdate(a->model, BR_MODU_VERTEX_POSITIONS);
		}

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
		 * Update display
		 */
		BrPixelmapDoubleBuffer(screenBuffer,backBuffer);

		/* 
		 * User controls
		 */
		if(Control(a))
			break;
	}

	/*
	 * Close down
	 */
	BrPixelmapFree(depthBuffer);
	BrPixelmapFree(backBuffer);

	DOSClockEnd();
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
int Control(br_actor *target)
{
	ControlObject(target);
	
	if(DOSKeyTest(SC_Q,0,REPT_FIRST_DOWN))
		return 1;

	if(DOSKeyTest(SC_SPACE,0,REPT_FIRST_DOWN))
		runMorph = !runMorph;

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
	
	BrPixelmapText(backBuffer, x * 4, y * 6, 255, NULL,  temp);
}

/*
 * Linear interpolation of vertices of two models into a destination
 */
void MorphModels(br_model *dest, br_model *model_a, br_model *model_b, br_scalar alpha)
{
	br_scalar beta = BR_SUB(BR_SCALAR(1.0),alpha);
	br_vertex *vp_dest,*vp_a,*vp_b;
	int v;
	
	vp_dest = dest->vertices;
	vp_a = model_a->vertices;
	vp_b = model_b->vertices;

	for(v=0; v < dest->nvertices; v++, vp_dest++, vp_a++, vp_b++) {
		vp_dest->p.v[0] = BR_MAC2(vp_a->p.v[0],beta,vp_b->p.v[0],alpha);
		vp_dest->p.v[1] = BR_MAC2(vp_a->p.v[1],beta,vp_b->p.v[1],alpha);
		vp_dest->p.v[2] = BR_MAC2(vp_a->p.v[2],beta,vp_b->p.v[2],alpha);
	}
}

/*
 * Piecewise linear morph along a sequence of evenly spaced targets
 */
void MorphModelsMany(br_model *dest, br_model **models, int nmodels, br_scalar alpha)
{
	br_scalar a;
	int mnum;
	
	if(nmodels <= 1)
		return;

	nmodels--;


	if(alpha <= BR_SCALAR(0.0)) {
		a = alpha;
		mnum = 0;
	} else if(alpha >= BR_SCALAR(1.0)) {
		a = alpha;
		mnum = nmodels-1;
	} else {
		mnum = BrScalarToInt(BR_MUL(alpha,BrIntToScalar(nmodels)));
		a = BR_SUB(BR_MUL(alpha,BrIntToScalar(nmodels)),BrIntToScalar(mnum));
	}

	MorphModels(dest,models[mnum],models[mnum+1],a);
}

