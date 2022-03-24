/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 * Program to Display a Revolving Texture Mapped Fork - 15-bit
 */
#include <stddef.h>
#include <stdio.h>
#include "brender.h"
#include "dosio.h"


int main()
{
   br_pixelmap *screen_buffer, *back_buffer, *depth_buffer;
   br_pixelmap *tile_pm;
   br_actor *world, *observer, *fork;
   int i;
   br_material *mats[10];
  
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

/*Load and Register refmap Texture */
        tile_pm = BrPixelmapLoad("refmap.pix");
	if (tile_pm==NULL)
                BR_ERROR("Couldn't load refmap.pix");
	BrMapAdd(tile_pm);

/*Load and Register fork Material */
	i = BrFmtScriptMaterialLoadMany("fork.mat",mats,BR_ASIZE(mats));
	BrMaterialAddMany(mats,i);                                      
	
/*Load and Position fork Actor*/
	fork = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	fork->model = BrModelLoad("fork.dat");
	BrModelAdd(fork->model);
	BrModelApplyMap(fork->model,BR_APPLYMAP_PLANE,NULL);
	fork->t.type = BR_TRANSFORM_MATRIX34;

/*Assign fork Material */
        fork->material = BrMaterialFind("CHROME GIFMAP");
/******************************* Animation Loop ****************************/                                                              
	  
	for(i=0;i<200;i++) {
		
		BrPixelmapFill(back_buffer,0);
		BrPixelmapFill(depth_buffer,0xFFFFFFFF);
		BrZbSceneRender(world,observer,back_buffer,depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer,back_buffer);
		
		BrMatrix34PostRotateY(&fork->t.t.mat,BR_ANGLE_DEG(2.0));
	       
			}                                           
/*Close down*/
	BrPixelmapFree(depth_buffer);
	BrPixelmapFree(back_buffer);
	BrZbEnd();
	DOSGfxEnd();
	BrEnd();

	return 0;
}


