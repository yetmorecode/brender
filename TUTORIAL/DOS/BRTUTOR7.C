/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 * Program to Display a Revolving Yellow Duck
 */
#include <stddef.h>
#include <stdio.h>
#include "brender.h"
#include "dosio.h"


int main()
{
   br_pixelmap *screen_buffer, *back_buffer, *depth_buffer;
   br_actor *world, *observer, *duck;
   br_material *mats[10];
   int i;

/*************** Initialize Renderer and Set Screen Resolution *************/
	BrBegin();
        screen_buffer = DOSGfxBegin("VESA,B:15");
	BrZbBegin(screen_buffer->type,BR_PMT_DEPTH_16);     
	back_buffer = BrPixelmapMatch(screen_buffer,BR_PMMATCH_OFFSCREEN);
        back_buffer->origin_x=back_buffer->width/2;
        back_buffer->origin_y=back_buffer->height/2;
	depth_buffer = BrPixelmapMatch(screen_buffer,BR_PMMATCH_DEPTH_16);
	 
/************************ Build the World Database *************************/

	world = BrActorAllocate(BR_ACTOR_NONE,NULL);
	BrLightEnable(BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT,NULL)));
    
/*Load and Position Camera*/        
	observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	observer->t.type = BR_TRANSFORM_MATRIX34;        
	BrMatrix34Translate(&observer->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(5.0));

/*Load and Apply Duck Materials*/
	i = BrFmtScriptMaterialLoadMany("duck.mat",mats,BR_ASIZE(mats));
	BrMaterialAddMany(mats,i);                                      
	
/*Load and Position Duck Actor*/
	duck = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	duck->model = BrModelLoad("duck.dat");
	BrModelAdd(duck->model);
	duck->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34RotateY(&duck->t.t.mat,BR_ANGLE_DEG(30));  

/******************************* Animation Loop ****************************/                                                              
	
	for(i=0;i<200;i++) {
		
		BrPixelmapFill(back_buffer,0);
		BrPixelmapFill(depth_buffer,0xFFFFFFFF);
		BrZbSceneRender(world,observer,back_buffer,depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer,back_buffer);
		
		BrMatrix34PostRotateX(&duck->t.t.mat,BR_ANGLE_DEG(2.0));
	       
			}                                           
/*Close down*/
	BrPixelmapFree(depth_buffer);
	BrPixelmapFree(back_buffer);
	BrZbEnd();
	DOSGfxEnd();
	BrEnd();

	return 0;
}


