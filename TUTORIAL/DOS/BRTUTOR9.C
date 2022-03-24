/** Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 * Program to Display a Chrome-Textured Fork (8-bit)
*/
#include <stddef.h>
#include <stdio.h>
#include "brender.h"
#include "dosio.h"

int main()
{
   br_pixelmap  *screen_buffer, *back_buffer, *depth_buffer, 
		*palette, *shade;
   br_actor *world, *observer, *fork;
   br_pixelmap *chrome_pm;
   int i;
/************** Initialize Renderer and Set Screen Resolution **************/
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
    /* Load Shade Table*/
	shade = BrPixelmapLoad("shade.tab");
	if (shade==NULL)
	   BR_ERROR0("Couldn't load shade.tab");
	BrTableAdd(shade);
/************************ Build the World Database *************************/
    
	world = BrActorAllocate(BR_ACTOR_NONE,NULL);
	BrLightEnable(BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT,NULL)));

/*Load and Position Camera Actor*/
	observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	observer->t.type = BR_TRANSFORM_MATRIX34;        
	BrMatrix34PostTranslate(&observer->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(5.0));
      
 /*Load and Register "chrome" Texture*/
	chrome_pm = BrPixelmapLoad("refmap.pix");
	if (chrome_pm==NULL)
		BR_ERROR0("Couldn't load refmap.pix");
	BrMapAdd(chrome_pm);
	 
/*Load and Apply "fork" Material*/
       BrMaterialAdd(BrFmtScriptMaterialLoad("fork.mat")); 
  
/*Load and Position fork Actor*/
	fork = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	fork->model = BrModelLoad("fork.dat");
	BrModelAdd(fork->model);
	BrModelApplyMap(fork->model,BR_APPLYMAP_PLANE,NULL);
	fork->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34RotateX(&fork->t.t.mat,BR_ANGLE_DEG(30));  
	fork->material = BrMaterialFind("CHROME GIFMAP"); 
/************************** Animation Loop ************************/                                                              
	
	for(i=0;i<200;i++) {
		BrPixelmapFill(back_buffer,0);
		BrPixelmapFill(depth_buffer,0xFFFFFFFF);
		BrZbSceneRender(world,observer,back_buffer,depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer,back_buffer);
		
		BrMatrix34PostRotateY(&fork->t.t.mat,BR_ANGLE_DEG(2.0));
			}
/* Close down */
	BrPixelmapFree(depth_buffer);
	BrPixelmapFree(back_buffer);
	BrZbEnd();
	DOSGfxEnd();
	BrEnd();

	return 0;
}


