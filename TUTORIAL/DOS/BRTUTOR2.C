/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 * Program to Display Scene containing a Box, a Sphere and a Torus
 */
#include <stddef.h>
#include <stdio.h>
#include "brender.h"
#include "dosio.h"


int main()
{

   br_pixelmap *screen_buffer, *back_buffer, *depth_buffer, *palette;
   br_actor *world, *observer, *box, *light, *sphere, *torus;
   int i;
   br_camera *camera_data;
/*************** Initialize Renderer and Set Screen Resolution **************/
	
   BrBegin();
   screen_buffer = DOSGfxBegin("MCGA");
   palette = BrPixelmapLoad("std.pal");
   if(palette)
	   DOSGfxPaletteSet(palette);
   BrZbBegin(screen_buffer->type,BR_PMT_DEPTH_16);   

   back_buffer = BrPixelmapMatch(screen_buffer,BR_PMMATCH_OFFSCREEN);
   back_buffer->origin_x=back_buffer->width/2;
   back_buffer->origin_y=back_buffer->height/2;

   depth_buffer = BrPixelmapMatch(screen_buffer,BR_PMMATCH_DEPTH_16);

/************************ Build the World Database **************************/

/*Load Root actor*/          
	world = BrActorAllocate(BR_ACTOR_NONE,NULL);

/*Load and Enable Default Light Source*/        
	light = BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT,NULL));
	BrLightEnable(light);

/*Load and Position Camera*/
	observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	observer->t.type = BR_TRANSFORM_MATRIX34;        
	BrMatrix34Translate(&observer->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(10.0));
	camera_data = (br_camera *)observer->type_data;
	camera_data->yon_z = BR_SCALAR(50);
      
/*Load and Position Box Model*/
	box = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	box->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34RotateY(&box->t.t.mat,BR_ANGLE_DEG(30));
	BrMatrix34PostTranslate(&box->t.t.mat,BR_SCALAR(-2.5),BR_SCALAR(0.0),BR_SCALAR(0.0));     
	BrMatrix34PreScale(&box->t.t.mat,BR_SCALAR(2.0),BR_SCALAR(1.0),BR_SCALAR(1.0));            
	
/*Load and Position Sphere Model*/
	sphere = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	sphere->model = BrModelLoad("sph32.dat");
	BrModelAdd(sphere->model);
	sphere->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34Translate(&sphere->t.t.mat,BR_SCALAR(2.0),BR_SCALAR(0.0),BR_SCALAR(0.0));     

/*Load and Position Torus Model*/
	torus = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	torus->model = BrModelLoad("torus.dat");
	BrModelAdd(torus->model);
	torus->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34Translate(&torus->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(3.0));     

/******************************* Animation Loop *****************************/                                                              
			
	for(i=0;i<360;i++) {
		
		BrPixelmapFill(back_buffer,0);
		BrPixelmapFill(depth_buffer,0xFFFFFFFF);

		BrZbSceneRender(world,observer,back_buffer,depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer,back_buffer);
		
		BrMatrix34PostRotateX(&box->t.t.mat,BR_ANGLE_DEG(2.0));
		  
		BrMatrix34PreRotateZ(&torus->t.t.mat,BR_ANGLE_DEG(4.0));
		BrMatrix34PreRotateY(&torus->t.t.mat,BR_ANGLE_DEG(-6.0));
		BrMatrix34PreRotateX(&torus->t.t.mat,BR_ANGLE_DEG(2.0)); 
		
		BrMatrix34PostRotateX(&torus->t.t.mat,BR_ANGLE_DEG(1.0));
		BrMatrix34PostRotateY(&sphere->t.t.mat,BR_ANGLE_DEG(0.8));
	}

	/*Close down*/

	BrPixelmapFree(depth_buffer);
	BrPixelmapFree(back_buffer);
	BrZbEnd();
	DOSGfxEnd();
	BrEnd();

	return 0;
}


