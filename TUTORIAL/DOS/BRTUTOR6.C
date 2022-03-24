/* Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 * Program to Display a Texture Mapped Sphere (15-bit colour)
 */
#include <stddef.h>
#include <stdio.h>
#include "brender.h"
#include "dosio.h"


int main()
{
   br_pixelmap  *screen_buffer, *back_buffer, *depth_buffer;
   
   br_actor *world, *observer, *planet;
   br_material *planet_material;

   int i;

/************** Initialize Renderer and Set Screen Resolution **************/
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

/*Load and Position Camera Actor*/
	observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	observer->t.type = BR_TRANSFORM_MATRIX34;        
	BrMatrix34PostTranslate(&observer->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(5.0));
      
/*Load and Position Planet Actor*/
	planet = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	planet->model = BrModelLoad("sph32.dat");
	BrModelAdd(planet->model);
	planet->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34RotateX(&planet->t.t.mat,BR_ANGLE_DEG(90));  
  
/*Load and Register "earth" Texture*/
	BrMapAdd(BrPixelmapLoad("earth15.pix"));

/*Load and Apply Earth Material*/
	planet_material = BrFmtScriptMaterialLoad("earth.mat");
	BrMaterialAdd(planet_material);
	planet->material = BrMaterialFind("earth_map");

/************************** Animation Loop ************************/                                                              
	
	for(i=0;i<200;i++) {
		BrPixelmapFill(back_buffer,0);
		BrPixelmapFill(depth_buffer,0xFFFFFFFF);
		BrZbSceneRender(world,observer,back_buffer,depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer,back_buffer);
		
		BrMatrix34PostRotateY(&planet->t.t.mat,BR_ANGLE_DEG(2.0));
	}
/* Close down */
	BrPixelmapFree(depth_buffer);
	BrPixelmapFree(back_buffer);
	BrZbEnd();
	DOSGfxEnd();
	BrEnd();

	return 0;
}


