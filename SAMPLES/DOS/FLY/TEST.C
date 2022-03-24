
/*
 * Copyright (c) 1993 Argonaut Software Ltd. All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <conio.h>
#include <string.h>

#include "brender.h"
#include "dosio.h"
#include "fmt.h"

#include "fly.h"

#define DEBUG_SHADOW 0

#define XSCALE BR_SCALAR(0.07194)
#define ZSCALE BR_SCALAR(-0.07194)
#define XOFFSET BR_SCALAR(0.48202)
#define ZOFFSET BR_SCALAR(0.421888)

#define S0 BR_SCALAR(0.0)
#define S1 BR_SCALAR(1.0)
#define S1024 BR_SCALAR(1024.0)

#define SWIDTH 15
#define SHEIGHT 15

#define CYCLES 25

static void SetupPlanes(struct flight *s) {
    FlPlaneEnable(s,0);
    FlSetPlane(s,0,S0,S1,S0);
    FlPlaneEnable(s,1);
    FlSetPlane(s,1,S0,-S1,S0);
}

void Control(struct flight *s) {
    unsigned long buttons;
    int i;
    static long mouse_x,mouse_y;
    static int auto_on = 1;

    DOSMouseRead(&mouse_x,&mouse_y,&buttons);

    if ((buttons & BR_MSM_BUTTONL) && (buttons & BR_MSM_BUTTONR)) {
	mouse_x = mouse_y = 0;
    } else {
	if (buttons & BR_MSM_BUTTONL)
	    FlLinearImpulse(s,
	        S0,S0,BR_SCALAR(-0.002));
	if (buttons & BR_MSM_BUTTONR)
	    FlLinearImpulse(s,
	        S0,S0,BR_SCALAR(0.001));
    }

    if (mouse_x) {
	FlAngularImpulse(s,S0,
	    BR_MULDIV(BrIntToScalar(mouse_x),BR_SCALAR(-0.0005),S1024),
	    BR_MULDIV(BrIntToScalar(mouse_x),BR_SCALAR(-0.0015),S1024));
    }
    if (mouse_y) {
	FlAngularImpulse(s,
	    BR_MULDIV(BrIntToScalar(mouse_y),BR_SCALAR(0.002),S1024),
	    S0,S0);
    }

    /*
      Check keys
    */

    if (DOSKeyTest(SC_A,0,0))
	FlLinearImpulse(s,S0,S0,BR_SCALAR(-0.0005));
    if (DOSKeyTest(SC_Z,0,0))
	FlLinearImpulse(s,S0,S0,BR_SCALAR(0.0005));
    if (DOSKeyTest(SC_C_LEFT,0,0))
	FlLinearImpulse(s,BR_SCALAR(-0.0005),S0,S0);
    if (DOSKeyTest(SC_C_RIGHT,0,0))
	FlLinearImpulse(s,BR_SCALAR(0.0005),S0,S0);
    if (DOSKeyTest(SC_C_UP,0,0))
	FlLinearImpulse(s,S0,BR_SCALAR(0.0005),S0);
    if (DOSKeyTest(SC_C_DOWN,0,0))
	FlLinearImpulse(s,S0,BR_SCALAR(-0.0005),S0);
    if (DOSKeyTest(SC_END,0,0))
	FlAngularImpulse(s,S0,BR_SCALAR(0.0005),S0);
    if (DOSKeyTest(SC_PG_DOWN,0,0))
	FlAngularImpulse(s,S0,BR_SCALAR(-0.0005),S0);
    if (DOSKeyTest(SC_L,0,REPT_FIRST_DOWN)) {
	auto_on = !auto_on;
    }
    if (auto_on)
	FlAutolevel(s);
}

br_pixelmap *shade;

#define HISTORY 30
#define LAG 5
#define REDUCE(n) (((n)+HISTORY)%HISTORY)

#define LENGTH 20000

/*
  When rendering shadow into terrain we save the unshadowed terrain
  temporarily.
*/

char saved_terrain[SWIDTH][SHEIGHT];

/*
  Render shadow into map at (u,v) using shade table and filtering down
  by averaging over 2x2 squares.
*/

void Shadow(br_pixelmap *map,br_pixelmap *shade,br_pixelmap *shadow,int u,int v) {
    int i,j;
    int r,s;
    char *ptr,*save = (char *)saved_terrain,pixel,*ptr2;

    for (i = 0; i<SWIDTH; i++)
	for (j = 0; j<SHEIGHT; j++) {
		ptr = (char *)map->pixels+(u+i & 1023)+(v+j & 1023)*map->row_bytes;
		*save++ = *ptr;
		ptr2 = (char *)shadow->pixels+2*i+2*j*shadow->row_bytes;
		pixel = ptr2[0]+ptr2[1];
		pixel += ptr2[shadow->row_bytes]+ptr2[shadow->row_bytes+1];
		if (pixel)
		    *ptr = ((char *)shade->pixels)[(48-pixel)*256+*ptr];
	}
}

/*
  Restore unshadowed terrain
*/

void UnShadow(br_pixelmap *map,int u,int v) {
    int i,j;
    char *ptr,*save = (char *)saved_terrain;

    for (i = 0; i<SWIDTH; i++)
	for (j = 0; j<SHEIGHT; j++) {
		ptr = (char *)map->pixels+(u+i & 1023)+(v+j & 1023)*map->row_bytes;
		*ptr = *save++;
	    }
}

br_matrix34 history[HISTORY];
br_matrix34 recording[LENGTH];

br_actor *craft,*craft_frame,*world;
br_actor *billboard,*house;
br_actor *terrain,*camera1,*camera2,*light1,*light2;
br_actor *shadow_camera;

void SetupTerrain(void) {
    terrain = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
    terrain->model = BrModelLoad("terrain5.dat");
    if (terrain->model==NULL)
	BR_ERROR0("Couldn't load terrain.dat");
    terrain->model->identifier = "terrain5";
    BrModelAdd(terrain->model);
    terrain->material = BrMaterialFind("tundra");
    BrMatrix34PostTranslate(&terrain->t.t.mat,
	BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(-1.0));

}

void SetupCraft(void) {

    /*
      The craft is attached to a 'craft frame'. We can then rotate
      the saucer freely without upsetting the flight dynamics
    */

    craft_frame = BrActorAdd(world,BrActorAllocate(BR_ACTOR_NONE,NULL));
    BrMatrix34PostTranslate(&craft_frame->t.t.mat,
	BR_SCALAR(0.0),BR_SCALAR(2.0),BR_SCALAR(0.0));

    craft = BrActorAdd(craft_frame,BrActorAllocate(BR_ACTOR_MODEL,NULL));
    craft->model = BrModelLoad("saucer.dat");
    if (craft->model==NULL)
	BR_ERROR0("Couldn't load saucer.dat");
    craft->model->identifier = "saucer";
    BrModelAdd(craft->model);
    craft->material = BrMaterialFind("bodywork");
    BrMatrix34PostRotateX(&craft->t.t.mat,BR_ANGLE_DEG(90));
    BrMatrix34PostRotateZ(&craft->t.t.mat,BR_ANGLE_DEG(180));
    BrMatrix34PostScale(&craft->t.t.mat,
	BR_SCALAR(0.1),BR_SCALAR(0.1),BR_SCALAR(0.1));
}

void SetupFurniture(void) {
    billboard = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
    billboard->model = BrModelLoad("billb.dat");
    if (billboard->model==NULL)
	BR_ERROR0("Couldn't load billb.dat");
    BrModelAdd(billboard->model);
    billboard->material = BrMaterialFind("logo");
    BrMatrix34PostRotateX(&billboard->t.t.mat,BR_ANGLE_DEG(90));
    BrMatrix34PostRotateZ(&billboard->t.t.mat,BR_ANGLE_DEG(180));
    BrMatrix34PostRotateY(&billboard->t.t.mat,BR_ANGLE_DEG(180));
    BrMatrix34PostScale(&billboard->t.t.mat,
	BR_SCALAR(0.2),BR_SCALAR(0.2),BR_SCALAR(0.2));
    BrMatrix34PostTranslate(&billboard->t.t.mat,
    	BR_SCALAR(-0.033),BR_SCALAR(0.27),BR_SCALAR(-3.10));
#if 0
    house = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
    house->model = BrModelLoad("house.dat");
    if (house->model==NULL)
	BR_ERROR0("Couldn't load house.dat");
    BrModelAdd(house->model);
    BrMatrix34PostRotateX(&house->t.t.mat,BR_ANGLE_DEG(90));
    BrMatrix34PostRotateZ(&house->t.t.mat,BR_ANGLE_DEG(180));
    BrMatrix34PostRotateY(&house->t.t.mat,BR_ANGLE_DEG(-20));
    BrMatrix34PostScale(&house->t.t.mat,
	BR_SCALAR(0.2),BR_SCALAR(0.2),BR_SCALAR(0.2));
    BrMatrix34PostTranslate(&house->t.t.mat,
    	BR_SCALAR(-3.95),BR_SCALAR(-0.67),BR_SCALAR(-4.58));
#endif
}

void SetupCameras(void) {

    /*
      Camera 1 is the tracking view that lags behind the saucer.
    */

    camera1 = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
    camera1->identifier = "camera1";
    ((br_camera *)camera1->type_data)->type = BR_CAMERA_PERSPECTIVE_OLD;
    ((br_camera *)camera1->type_data)->field_of_view = BR_ANGLE_DEG(55.0);
    ((br_camera *)camera1->type_data)->hither_z = BR_SCALAR(0.01); 
    ((br_camera *)camera1->type_data)->yon_z = BR_SCALAR(100.0); 
    ((br_camera *)camera1->type_data)->aspect = BR_SCALAR(1.6); 

    /*
      Camera 2 is the view from the saucer itself.
    */

    camera2 = BrActorAdd(craft_frame,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
    camera2->identifier = "camera2";
    ((br_camera *)camera2->type_data)->type = BR_CAMERA_PERSPECTIVE_OLD;
    ((br_camera *)camera2->type_data)->field_of_view = BR_ANGLE_DEG(45.0);
    ((br_camera *)camera2->type_data)->hither_z = BR_SCALAR(0.01); 
    ((br_camera *)camera2->type_data)->yon_z = BR_SCALAR(20.0); 
    ((br_camera *)camera2->type_data)->aspect = BR_SCALAR(1.6); 
    BrMatrix34PostTranslate(&camera2->t.t.mat,S0,S0,BR_SCALAR(-0.1));

    /*
      The shadow camera is used to capture the silhouette of the
      saucer
    */

    shadow_camera = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
    shadow_camera->identifier = "shadow_camera";
    ((br_camera *)shadow_camera->type_data)->type = BR_CAMERA_PARALLEL_OLD;
    ((br_camera *)shadow_camera->type_data)->width = BR_SCALAR(0.1);
    ((br_camera *)shadow_camera->type_data)->height = BR_SCALAR(0.1);
    ((br_camera *)shadow_camera->type_data)->hither_z = BR_SCALAR(0.01); 
    ((br_camera *)shadow_camera->type_data)->yon_z = BR_SCALAR(1.0); 
}

void SetupLights(void) {
    light1 = BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT,NULL));
    light1->identifier = "light1";
    ((br_light *)light1->type_data)->type = BR_LIGHT_DIRECT;
    ((br_light *)light1->type_data)->attenuation_c = BR_SCALAR(1.0);
    BrMatrix34RotateX(&light1->t.t.mat,BR_ANGLE_DEG(-45));
    BrMatrix34RotateZ(&light1->t.t.mat,BR_ANGLE_DEG(-45));
    BrMatrix34RotateY(&light1->t.t.mat,BR_ANGLE_DEG(180));
    BrLightEnable(light1);

    /*
      Attach a spotlight to the front of the saucer
    */

    light2 = BrActorAdd(craft_frame,BrActorAllocate(BR_ACTOR_LIGHT,NULL));
    light2->identifier = "light2";
    ((br_light *)light2->type_data)->type = BR_LIGHT_SPOT;
    ((br_light *)light2->type_data)->attenuation_c = S1;
    BrLightEnable(light2);

}

main(int argc,char *argv[]) {
    br_pixelmap *depth_buffer,*tundra_pm,*palette;
    br_pixelmap *shadow_buffer,*shadow_depth;
    br_pixelmap *bodywork_pm;
    br_matrix34 rotation;
    br_pixelmap *screen,*back_screen,*panel,*window;
    br_colour *pal_entry;
    int last_time,this_time;
    int hp = 0;
    int n;
    br_material *mats[20];
    int fp = 0;
    char *filename;
    void *file;
    int opentype = BR_FS_MODE_BINARY;
    int Recording = 0,Playing = 0;
    int length;
    int track_view = 1;
    int shadow_u,shadow_v;
    br_scalar x,z;

    struct flight s;

    int i,dx,dy,buttons;

    BR_BANNER("UFO Flight Demo","1994-1995","$Revision: 1.1 $");
#ifndef _MSC_VER
    sleep(2);
#endif

    /*
      Does the user wish to record or play back a demo?
    */

    if (argc==3) {
	if (!strcmp(argv[1],"-r")) {
	    Recording = 1;
	    filename = argv[2];
	}
	if (!strcmp(argv[1],"-p")) {
	    Playing = 1;
	    filename = argv[2];
	}
    }

    /*
      Open file for playback
    */

    BrBegin();

    if (Playing) {
	file = BrFileOpenRead(filename,0,NULL,&opentype);
	if (file==NULL)
	    BR_ERROR1("Couldn't open file '%s' for reading",filename);
	BrFileRead(&length,sizeof(int),1,file);
	if (length>LENGTH)
	    length = LENGTH;
	BrFileRead(recording,sizeof(br_matrix34),length,file);
	BrFileClose(file);
    }

    /*
      Set up initial history data
    */

    for (i = 0; i<HISTORY; i++)
	BrMatrix34Identity(history+i);


    /* Initialise BRender */

    DOSKeyBegin();
    DOSMouseBegin();
    DOSClockBegin();

    screen = DOSGfxBegin(NULL);
    back_screen = BrPixelmapMatch(screen,BR_PMMATCH_OFFSCREEN);
    window = BrPixelmapAllocateSub(back_screen,
	0,0,back_screen->width,back_screen->height-30);
    panel = BrPixelmapAllocateSub(back_screen,
	0,back_screen->height-30,back_screen->width,30);
    depth_buffer = BrPixelmapMatch(window,BR_PMMATCH_DEPTH_16);

    /* 
      We find the silhouette of the saucer for shadowing by rendering
      into shadow_buffer. In fact we oversample 2x2 times so we can
      filter it down to attractive gray shades
    */

    shadow_buffer = BrPixelmapAllocate(BR_PMT_INDEX_8,SWIDTH*2,SHEIGHT*2,NULL,BR_PMAF_NORMAL);
    shadow_depth = BrPixelmapMatch(shadow_buffer,BR_PMMATCH_DEPTH_16);

    BrZbBegin(window->type,depth_buffer->type);

    /*
      Install textures. Any pixelmap identifier not in registry is
      assumed to refer to the file containing it
    */

    BrTableFindHook(BrTableFindFailedLoad);
    BrMapFindHook(BrMapFindFailedLoad);

    n = BrFmtScriptMaterialLoadMany("fly.mat",mats,BR_ASIZE(mats));
    BrMaterialAddMany(mats,n);

    /* Load and install palette */

    palette = BrPixelmapLoad("fly.pal");

    DOSGfxPaletteSet(palette);

    world = BrActorAllocate(BR_ACTOR_NONE,NULL);

    SetupTerrain();
    SetupCraft();
    SetupFurniture();
    SetupCameras();
    SetupLights();

    if (!Playing) {
	FlInit(&s);
	SetupPlanes(&s);
	FlSetPosition(&s,&craft_frame->t.t.mat);
    }

    last_time = DOSClockRead();

    while (!DOSKeyTest(SC_ESC,0,0) & fp<LENGTH) {
	int i,cycles;

	this_time = DOSClockRead();
	cycles = CYCLES*(this_time-last_time)/BR_DOS_CLOCK_RATE;

	if (Playing) {
	    if (fp>=length)
		fp = 0;
	    BrMatrix34Copy(&craft_frame->t.t.mat,recording+fp++);
	} else {
	    for (i = 0; i<cycles; i++) {
		Control(&s);
		FlEvolve(&s,S1);
		FlTransformFromState(&craft_frame->t.t.mat,&s);
	    }
	    BrMatrix34LPNormalise(&craft_frame->t.t.mat,&craft_frame->t.t.mat);
	}

	BrMatrix34PreRotateZ(&craft->t.t.mat,BR_ANGLE_DEG(4));

	/*
	  We must ensure that the saucer always remains transformed by
	  an orthogonal transformation that maintains it at the correct
	  size
	*/

	BrMatrix34PostScale(&craft->t.t.mat,
	    BR_SCALAR(10),BR_SCALAR(10),BR_SCALAR(10));
	BrMatrix34LPNormalise(&craft->t.t.mat,&craft->t.t.mat);
	BrMatrix34PostScale(&craft->t.t.mat,
	    BR_SCALAR(0.1),BR_SCALAR(0.1),BR_SCALAR(0.1));

	if (Recording) 
	    BrMatrix34Copy(recording+fp++,&craft_frame->t.t.mat);

	last_time = last_time+cycles*BR_DOS_CLOCK_RATE/CYCLES;

	/*
	  The tracking camera comes from the position of the saucer
	  a few frames earlier - displaced backwards slightly
	*/

	BrMatrix34Copy(&history[REDUCE(hp)],&craft_frame->t.t.mat);
	BrMatrix34Copy(&camera1->t.t.mat,&history[REDUCE(hp-5)]);
	BrMatrix34PreTranslate(&camera1->t.t.mat,S0,BR_SCALAR(0.05),BR_SCALAR(0.3));
	hp++;

	/* Clear off screen buffer and its Z buffer */

	BrPixelmapFill(back_screen,0);

	BrPixelmapFill(depth_buffer,0xffffffff);

	if (DOSKeyTest(SC_V,0,REPT_FIRST_DOWN))
	    track_view = !track_view;

	if (DOSKeyTest(SC_S,0,REPT_FIRST_DOWN)) {
	    BrActorSave("fly.dat",world);
	    BrMaterialSave("tundra.mat",BrMaterialFind("tundra"));
	}

	/* Render shadow into shadow_buffer */

	/* First place camera directly beneath saucer looking up */

	BrPixelmapFill(shadow_depth,0xffffffff);
	BrPixelmapFill(shadow_buffer,0);

	/*
	  Render the shadow...

	  First place camera directly below saucer facing upwards.
	*/

	BrMatrix34RotateX(&shadow_camera->t.t.mat,BR_ANGLE_DEG(90));
	BrMatrix34PostTranslate(&shadow_camera->t.t.mat,
	    craft_frame->t.t.mat.m[3][0],
	    craft_frame->t.t.mat.m[3][1]-BR_SCALAR(0.1),
	    craft_frame->t.t.mat.m[3][2]);

	/*
	  We use a dedicated 'shadow' material for the saucer to the
	  the silhouette.
	*/

	craft->material = BrMaterialFind("shadow");

	/* 
	  We only wish to view the saucer so turn off everything else
	*/

	terrain->render_style = BR_RSTYLE_NONE;
	billboard->render_style = BR_RSTYLE_NONE;
#if 0
	house->render_style = BR_RSTYLE_NONE;
#endif

	/*
	  Render shadow 2x2 oversampled
	*/

	BrZbSceneRender(world,shadow_camera,shadow_buffer,shadow_depth);
	terrain->render_style = BR_RSTYLE_DEFAULT;
	billboard->render_style = BR_RSTYLE_DEFAULT;
#if 0
	house->render_style = BR_RSTYLE_DEFAULT;
#endif
	/*
	  Restore saucer material
	*/

	craft->material = BrMaterialFind("bodywork");

	x = BR_ADD(BR_MUL(craft_frame->t.t.mat.m[3][0],XSCALE),XOFFSET);
	z = BR_ADD(BR_MUL(craft_frame->t.t.mat.m[3][2],ZSCALE),ZOFFSET);
	shadow_u = BrScalarToInt(BR_MUL(S1024,x));
	shadow_v = BrScalarToInt(BR_MUL(S1024,z));

	/*
	  Now put shadow into terrain texture
	*/

	Shadow(terrain->material->colour_map,terrain->material->index_shade,
	    shadow_buffer,
	    shadow_u,shadow_v);

	/*
	  Render the whole frame
	*/

	BrZbSceneRender(world,
	    track_view ? camera1 : camera2,
	    window,depth_buffer);

	/*
	  Restore terrain
	*/

	UnShadow(terrain->material->colour_map,shadow_u,shadow_v);

	BrPixelmapTextF(panel,3,3,122,BrFontProp7x9,"Location: %f %f %f",
	    BrScalarToFloat(craft_frame->t.t.mat.m[3][0]),
	    BrScalarToFloat(craft_frame->t.t.mat.m[3][1]),
	    BrScalarToFloat(craft_frame->t.t.mat.m[3][2])
	);
	if (Recording)
	    BrPixelmapTextF(panel,3,17,97,BrFontProp7x9,"Recording - frame: %d",fp);
	if (Playing)
	    BrPixelmapTextF(panel,3,17,97,BrFontProp7x9,"Playback - frame: %d",fp);

#if DEBUG_SHADOW
	BrPixelmapRectangleCopy(back_screen,0,0,shadow_buffer,0,0,SWIDTH*2,SHEIGHT*2);
#endif

	/* Copy off screen buffer to display */

	BrPixelmapDoubleBuffer(screen,back_screen);
    }

    if (Recording) {
	file = BrFileOpenWrite(filename,BR_FS_MODE_BINARY);
	BrFileWrite(&fp,sizeof(int),1,file);
	BrFileWrite(recording,sizeof(br_matrix34),fp,file);
	BrFileClose(file);
    }

    DOSKeyEnd();
    DOSMouseEnd();
    BrZbEnd();
    DOSGfxEnd();
    BrEnd();

	return 0;
}
