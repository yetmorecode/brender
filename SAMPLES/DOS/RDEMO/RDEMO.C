/*
 * File:RDEMO.C
 * 
 * Copyright(c) 1995 Argonaut Software Ltd.All rights reserved.
 * 
 * Description:Teapot in space.
 * 
 * Simulated reflections by rendering into environment maps supports high and
 * low resloution 640 x480 and 320 x200
 * 
 * Author:Dan Piponi
 * 
 * Modifications:Tony Roberts Added intro screen_buffer... Steve Williams Tidied
 * up some stuff.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

#include "brender.h"
#include "dosio.h"
#include "string.h"

#define PmFree(x) if(x)BrPixelmapFree(x)

br_pixelmap *screen_buffer = NULL;
br_pixelmap *depth_buffer = NULL;
br_pixelmap *colour_buffer = NULL;
br_pixelmap *palette = NULL;

/* This demo simulates a giant reflecting teapot hanging above a */
/* spinning earth */

int RotateTeapot = 0;
int moon_choice = 0;
int earth_choice = 0;

br_boolean Change_Screen_Res(void)
{
	BrZbEnd();
	DOSGfxEnd();

	PmFree(colour_buffer);
	PmFree(depth_buffer);

	if (screen_buffer->width == 320) {
		PmFree(screen_buffer);
		screen_buffer = DOSGfxBegin("VESA,W:640,H:480,B:8");
	} else {
		PmFree(screen_buffer);
		screen_buffer = DOSGfxBegin("MCGA");
	}

	BrZbBegin(screen_buffer->type, BR_PMT_DEPTH_16);
	colour_buffer = BrPixelmapMatch(screen_buffer, BR_PMMATCH_OFFSCREEN);
	depth_buffer = BrPixelmapMatch(screen_buffer, BR_PMMATCH_DEPTH_16);
	BrPixelmapFill(colour_buffer, 0x0);
	BrPixelmapFill(depth_buffer, 0xffffffff);

	DOSGfxPaletteSet(palette);

	return BR_TRUE;
}

main()
{
	br_actor *world, *teapot, *earth, *moon, *camera, *mirror_view, *light1, *env;
	br_actor *pair, *system, *light2;
	br_pixelmap *mirror_depth, *shade, *earth_pm, *moon_pm;
	br_pixelmap *mirror_pm, *envmap1_pm;
	br_colour *pal_entry;
	br_matrix34 rotation;
	br_material *moon_map, *earth_map, *mirror;
	br_angle theta = 0;
	br_material *mats[20], *mat;
	int i, n;

	long mouse_x, mouse_y;
	unsigned long mouse_buttons;

	BR_BANNER("Reflection Demo", "1994-1995", "$Revision: 1.2 $");
#ifndef _MSC_VER
	sleep(2);
#endif

	/* Initialise BRender */

	BrBegin();
	screen_buffer = DOSGfxBegin(NULL);
	colour_buffer = BrPixelmapMatch(screen_buffer, BR_PMMATCH_OFFSCREEN);
	depth_buffer = BrPixelmapMatch(colour_buffer, BR_PMMATCH_DEPTH_16);

	BrZbBegin(screen_buffer->type, BR_PMT_DEPTH_16);

	/*
	 * Load up materials
	 */
	BrTableFindHook(BrTableFindFailedLoad);
	BrMapFindHook(BrMapFindFailedLoad);

	n = BrFmtScriptMaterialLoadMany("rdemo.mat", mats, BR_ASIZE(mats));
	BrMaterialAddMany(mats, n);

	/*
	 * Fix up the 'mirror' material to reference the rendered texture
	 */

	/* Load a shade table */

	shade = BrPixelmapLoad("shade.tab");

	if (shade == NULL)
		BR_ERROR0("Could't load shade.tab");

	/* Put shade table into registry */

	BrTableAdd(shade);

	/* Complete definition of mirror texture. Note that the colour_map */
	/* is a newly allocated pixelmap into which we'll render later */

	mirror = BrMaterialFind("mirror");
	mirror->colour_map = mirror_pm = BrPixelmapAllocate(BR_PMT_INDEX_8, 256, 256, NULL, 0);
	BrMapAdd(mirror_pm);
	BrMaterialUpdate(mirror, BR_MATU_ALL);

	mirror_depth = BrPixelmapMatch(mirror_pm, BR_PMMATCH_DEPTH_16);

	envmap1_pm = BrPixelmapLoad("testenv1.pix");
	if(envmap1_pm == NULL)
		BR_ERROR0("Could't load testenv1.pix");
		
	/* Load and install palette */

	palette = BrPixelmapLoad("std.pal");

	DOSGfxPaletteSet(palette);

	/* In the beginning... */

	world = BrActorAllocate(BR_ACTOR_NONE, NULL);

	/* The earth-teapot system forms a single `pair' unit */

	pair = BrActorAdd(world, BrActorAllocate(BR_ACTOR_NONE, NULL));
	pair->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34Identity(&pair->t.t.mat);

	system = BrActorAdd(pair, BrActorAllocate(BR_ACTOR_NONE, NULL));
	system->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34PostTranslate(&system->t.t.mat,
							BR_SCALAR(-0.7), BR_SCALAR(0), BR_SCALAR(0));

	/* Load teapot and place into `pair' */

	teapot = BrActorAdd(pair, BrActorAllocate(BR_ACTOR_MODEL, NULL));
	teapot->model = BrModelLoad("teapot.dat");
	if (teapot->model == NULL)
		BR_ERROR0("Couldn't load teapot.dat");
	BrModelAdd(teapot->model);
	teapot->material = BrMaterialFind("mirror");
	teapot->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34Scale(&teapot->t.t.mat,
					BR_SCALAR(0.7), BR_SCALAR(0.7), BR_SCALAR(0.7));
	BrMatrix34PostRotateX(&teapot->t.t.mat, BR_ANGLE_DEG(-70));
	BrMatrix34PostRotateY(&teapot->t.t.mat, BR_ANGLE_DEG(-45));
	BrMatrix34PostTranslate(&teapot->t.t.mat,
							BR_SCALAR(0.7), BR_SCALAR(0), BR_SCALAR(0));

	/* Load and place spheres into earth-moon system */

	earth = BrActorAdd(system, BrActorAllocate(BR_ACTOR_MODEL, NULL));
	earth->model = BrModelLoad("sph32.dat");
	if (earth->model == NULL)
		BR_ERROR0("Couldn't load sph32.dat");
	BrModelAdd(earth->model);
	earth->material = BrMaterialFind("earth");
	earth->t.type = BR_TRANSFORM_MATRIX34;

	moon = BrActorAdd(system, BrActorAllocate(BR_ACTOR_MODEL, NULL));
	moon->model = earth->model;
	moon->material = BrMaterialFind("moon");
	moon->t.type = BR_TRANSFORM_MATRIX34;

	/* Now place the main camera from which we will view the system */

	camera = BrActorAdd(world, BrActorAllocate(BR_ACTOR_CAMERA, NULL));
	((br_camera *) camera->type_data)->type = BR_CAMERA_PERSPECTIVE_OLD;
	((br_camera *) camera->type_data)->field_of_view = BR_ANGLE_DEG(45.0);
	((br_camera *) camera->type_data)->hither_z = BR_SCALAR(0.1);
	((br_camera *) camera->type_data)->yon_z = BR_SCALAR(20.0);
	((br_camera *) camera->type_data)->aspect = BR_SCALAR(1.46);
	camera->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34Translate(&camera->t.t.mat,
						BR_SCALAR(0.0), BR_SCALAR(0.0), BR_SCALAR(2.0));

	/* This is the camera that will be used to provide the reflection */
	/* of the earth into the teapot */

	mirror_view = BrActorAdd(pair, BrActorAllocate(BR_ACTOR_CAMERA, NULL));
	((br_camera *) mirror_view->type_data)->type = BR_CAMERA_PERSPECTIVE_OLD;
	((br_camera *) mirror_view->type_data)->field_of_view = BR_ANGLE_DEG(90.0);
	((br_camera *) mirror_view->type_data)->hither_z = BR_SCALAR(0.1);
	((br_camera *) mirror_view->type_data)->yon_z = BR_SCALAR(20.0);
	((br_camera *) mirror_view->type_data)->aspect = BR_SCALAR(1.46);
	mirror_view->t.type = BR_TRANSFORM_MATRIX34;

	/* Rotate the camera to face the earth... */

	BrMatrix34RotateY(&mirror_view->t.t.mat, BR_ANGLE_DEG(90));

	/* ...and move it into the centre of the teapot */

	BrMatrix34PostTranslate(&mirror_view->t.t.mat,
							BR_SCALAR(0.5), BR_SCALAR(0), BR_SCALAR(0));

	/* Let there be light...so we can see what we are doing */

	light1 = BrActorAdd(world, BrActorAllocate(BR_ACTOR_LIGHT, NULL));
	((br_light *) light1->type_data)->type = BR_LIGHT_DIRECT;
	((br_light *) light1->type_data)->attenuation_c = BR_SCALAR(1.0);
	light1->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34RotateY(&light1->t.t.mat, BR_ANGLE_DEG(35));
	BrMatrix34RotateX(&light1->t.t.mat, BR_ANGLE_DEG(35));

	light2 = BrActorAdd(world, BrActorAllocate(BR_ACTOR_LIGHT, NULL));
	((br_light *) light2->type_data)->type = BR_LIGHT_DIRECT;
	((br_light *) light2->type_data)->attenuation_c = BR_SCALAR(1.0);
	light2->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34RotateY(&light2->t.t.mat, BR_ANGLE_DEG(-35));
	BrMatrix34RotateX(&light2->t.t.mat, BR_ANGLE_DEG(-90));

	/* Mustn't forget to turn the light on */

	BrLightEnable(light1);
	BrLightEnable(light2);

	/* The environment map is attached to the earth-teapot pair so that */
	/* when rotated the reflection rotates accordingly */

	env = BrActorAdd(pair, BrActorAllocate(BR_ACTOR_NONE, NULL));
	env->t.type = BR_TRANSFORM_MATRIX34;

	/* Rotate environment map so that the centre of it is in the same */
	/* direction as the earth */

	BrMatrix34RotateY(&env->t.t.mat, BR_ANGLE_DEG(-90));
	BrEnvironmentSet(env);
	DOSMouseBegin();
	DOSKeyBegin();

	while (1) {
		/* Clear buffer and its Z buffer */

		BrPixelmapFill(colour_buffer, 0);
		BrPixelmapFill(depth_buffer, 0xffffffff);

		/* Copy plain environment map into mirror pixelmap. We will render */
		/* the earth over this. Also clear the Z buffer. */

		BrPixelmapCopy(mirror_pm, envmap1_pm);
		BrPixelmapFill(mirror_depth, 0xffffffff);

		/* When rendering the earth into the reflection we don't want to */
		/* see a view of the inside of the teapot. Turn it off for now */

		teapot->render_style = BR_RSTYLE_NONE;

		/* Render reflection and turn back on teapot */
		BrZbSceneRender(world, mirror_view, mirror_pm, mirror_depth);
		teapot->render_style = BR_RSTYLE_DEFAULT;

		/* Place rotated earth and moon */

		++theta;

		BrMatrix34Scale(&earth->t.t.mat,
						BR_SCALAR(0.35), BR_SCALAR(0.35), BR_SCALAR(0.35));
		BrMatrix34PostRotateX(&earth->t.t.mat, BR_ANGLE_DEG(90));
		BrMatrix34PreRotateZ(&earth->t.t.mat, BR_ANGLE_DEG(-3 * theta));

		BrMatrix34Scale(&moon->t.t.mat,
						BR_SCALAR(0.1), BR_SCALAR(0.1), BR_SCALAR(0.1));
		BrMatrix34PostTranslate(&moon->t.t.mat,
								BR_SCALAR(-0.7), BR_SCALAR(0), BR_SCALAR(0));
		BrMatrix34PostRotateY(&moon->t.t.mat, BR_ANGLE_DEG(-2 * theta));
		/*
		 * first time around while loop camera depth of field is set to zero
		 * so nothing is rendered and only backdrop is visible
		 */

		/* And now render the earth along with the shiny teapot */

		BrZbSceneRender(world, camera, colour_buffer, depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer, colour_buffer);

		/* Allow user to move the system around */

		mouse_x = mouse_y = 0;
		DOSMouseRead(&mouse_x, &mouse_y, &mouse_buttons);


		if (RotateTeapot == 1) {
			BrMatrix34RollingBall(&rotation, mouse_x, -mouse_y, 800);
			BrMatrix34Pre(&teapot->t.t.mat, &rotation);
		} else {
			if (mouse_x || mouse_y) {
				if (mouse_buttons & BR_MSM_BUTTONL) {
					BrMatrix34PostTranslate(&pair->t.t.mat,
								BR_MUL(BR_SCALAR(mouse_x), BR_SCALAR(0.01)),
						   BR_MUL(BR_SCALAR(-mouse_y), BR_SCALAR(0.01)), 0);
				} else if (mouse_buttons & BR_MSM_BUTTONR) {
					BrMatrix34PostTranslate(&pair->t.t.mat,
						0, 0, BR_MUL(BR_SCALAR(-mouse_y), BR_SCALAR(0.01)));
				} else {
					BrMatrix34RollingBall(&rotation, mouse_x, -mouse_y, 800);
					BrMatrix34Pre(&pair->t.t.mat, &rotation);
				}
			}
		}

		if(DOSKeyTest(SC_Q,0,REPT_FIRST_DOWN))
			break;

		if(DOSKeyTest(SC_P,0,REPT_FIRST_DOWN))
			BrPixelmapSave("scrn_dmp.pix", colour_buffer);

		if(DOSKeyTest(SC_T,0,REPT_FIRST_DOWN))
			RotateTeapot = !RotateTeapot;

		if(DOSKeyTest(SC_R,0,REPT_FIRST_DOWN)) {
			BrMatrix34Identity(&pair->t.t.mat);
			BrMatrix34Identity(&teapot->t.t.mat);
			BrMatrix34Scale(&teapot->t.t.mat,
						BR_SCALAR(0.7), BR_SCALAR(0.7), BR_SCALAR(0.7));
			BrMatrix34PostRotateX(&teapot->t.t.mat, BR_ANGLE_DEG(-70));
			BrMatrix34PostRotateY(&teapot->t.t.mat, BR_ANGLE_DEG(-45));
			BrMatrix34PostTranslate(&teapot->t.t.mat,
							BR_SCALAR(0.7), BR_SCALAR(0), BR_SCALAR(0));
		}

		if(DOSKeyTest(SC_M,0,REPT_FIRST_DOWN)) {
				moon_choice = !moon_choice;

				mat = BrMaterialFind("moon");
				if (mat == NULL)
					break;

				if (mat->colour_map) {
					BrMapRemove(mat->colour_map);
					BrPixelmapFree(mat->colour_map);
					mat->colour_map = NULL;
				}
				mat->colour_map = BrPixelmapLoad(moon_choice ? "moon.pix" : "ast.pix");
				if (mat->colour_map)
					BrMapAdd(mat->colour_map);

				BrMaterialUpdate(mat, BR_MATU_ALL);
		}

		if(DOSKeyTest(SC_E,0,REPT_FIRST_DOWN)) {
				earth_choice = !earth_choice;

				mat = BrMaterialFind("earth");
				if (mat == NULL)
					break;

				if (mat->colour_map) {
					BrMapRemove(mat->colour_map);
					BrPixelmapFree(mat->colour_map);
					mat->colour_map = NULL;
				}
				mat->colour_map = BrPixelmapLoad(earth_choice ? "planet.pix" : "earth.pix");
				if (mat->colour_map)
					BrMapAdd(mat->colour_map);

				BrMaterialUpdate(mat, BR_MATU_ALL);
		}

		if(DOSKeyTest(SC_S, 0, REPT_FIRST_DOWN))
				Change_Screen_Res();
	}

	BrZbEnd();

	DOSGfxEnd();
	DOSKeyEnd();
	DOSMouseEnd();

	BrEnd();

	return 0;
}
