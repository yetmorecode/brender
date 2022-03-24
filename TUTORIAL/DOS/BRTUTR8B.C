/* Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 * Program to Display a Texture Mapped dUCK (8-bit) 
*/
#include <stddef.h>
#include <stdio.h>
#include "brender.h"
#include "dosio.h"

int main()
{
   br_pixelmap  *screen_buffer, *back_buffer, *depth_buffer, 
		*palette, *shade;
   br_actor *world, *observer, *duck;
   br_pixelmap *gold_pm;
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
/************************ Build thE World Database *************************/
    
	world = BrActorAllocate(BR_ACTOR_NONE,NULL);
	BrLightEnable(BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT,NULL)));

/*Load and Position Camera Actor*/
	observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	observer->t.type = BR_TRANSFORM_MATRIX34;        
	BrMatrix34PostTranslate(&observer->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(5.0));
      
 /*Load and Register "gold" Texture*/
	gold_pm = BrPixelmapLoad("gold8.pix");
	if (gold_pm==NULL)
		BR_ERROR0("Couldn't load gold8.pix");
	BrMapAdd(gold_pm);
	 
/*Load and Apply "gold" Material*/
       BrMaterialAdd(BrFmtScriptMaterialLoad("gold8.mat"));                        

/*Load and Position duck Actor*/
	duck = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	duck->model = BrModelLoad("duck.dat");
	BrModelAdd(duck->model);
	BrModelApplyMap(duck->model,BR_APPLYMAP_PLANE,NULL);
	duck->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34RotateX(&duck->t.t.mat,BR_ANGLE_DEG(30));  
	duck->material = BrMaterialFind("gold8");   
/************************** Animation Loop ************************/                                                              
	
	for(i=0;i<200;i++) {
		BrPixelmapFill(back_buffer,0);
		BrPixelmapFill(depth_buffer,0xFFFFFFFF);
		BrZbSceneRender(world,observer,back_buffer,depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer,back_buffer);
		
		BrMatrix34PostRotateY(&duck->t.t.mat,BR_ANGLE_DEG(2.0));
			}
/* Close down */
	BrPixelmapFree(depth_buffer);
	BrPixelmapFree(back_buffer);
	BrZbEnd();
	DOSGfxEnd();
	BrEnd();

	return 0;
}


