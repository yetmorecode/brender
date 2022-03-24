/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 * Program to Display a Revolving Illuminated Cube
 */
#include <stddef.h>
#include <stdio.h>

#include "brender.h"
#include "dosio.h"

int main()
{
   /*
   ** Need screen and back buffers for double-buffering, 
   ** a Z-buffer to store current depth information, and a storage 
   ** buffer for the currently loaded palette
   */
   
   br_pixelmap *screen_buffer, *back_buffer, *depth_buffer, *palette;
   
   /* The actors in the world : Need a root actor, a camera actor, a light actor, 
   ** and a model actor
   */
   
   br_actor *world, *observer, *light, *cube;

   int i;                                       /*counter*/

/************* Initialize Renderer and Set Screen Resolution ***************/
   BrBegin();        
		
		/*Initialize screen buffer*/
   screen_buffer = DOSGfxBegin("MCGA");

		/* Set up CLUT (ignored in true-colour)*/
   palette = BrPixelmapLoad("std.pal");
   if(palette)
	DOSGfxPaletteSet(palette);
 
		/*Initialize z-buffer renderer*/
   BrZbBegin(screen_buffer->type,BR_PMT_DEPTH_16);
		
		/* Allocate back buffer and depth buffer*/
   
   back_buffer = BrPixelmapMatch(screen_buffer,BR_PMMATCH_OFFSCREEN);
   back_buffer->origin_x=back_buffer->width/2;
   back_buffer->origin_y=back_buffer->height/2;
   depth_buffer = BrPixelmapMatch(screen_buffer,BR_PMMATCH_DEPTH_16);

/************************* Build th World Database ************************/
	/*Start with None actor at root of actor tree and call it 'world'*/
   world = BrActorAllocate(BR_ACTOR_NONE,NULL);

	/* Add a camera actor as a child of 'world'*/
   observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));

	/*Add, and enable, the default light source*/
   light = BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT,NULL));
   BrLightEnable(light);
	
	/* Move camara position 5 units along +z axis so object is visible*/
   observer->t.type = BR_TRANSFORM_MATRIX34;        
   BrMatrix34PostTranslate(&observer->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(5.0));
      
	/*Add a model actor : The default unit cube*/
   cube = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));

	/*Rotate cube to enhance visiblilty*/
   cube->t.type = BR_TRANSFORM_MATRIX34;
   BrMatrix34RotateY(&cube->t.t.mat,BR_ANGLE_DEG(30));  

/********************************* Animation Loop **************************/                                                           
	
	/* Rotate cube around the x-axis */
   for(i=0;i<360;i++) {
	       
	/* Initialize depth buffer and set background colour to black*/
      BrPixelmapFill(back_buffer,0);
      BrPixelmapFill(depth_buffer,0xFFFFFFFF);

	/* Render scene */
      BrZbSceneRender(world,observer,back_buffer,depth_buffer);
      BrPixelmapDoubleBuffer(screen_buffer,back_buffer);
		
	/* Rotate cube */             
      BrMatrix34PostRotateX(&cube->t.t.mat,BR_ANGLE_DEG(2.0));
	       
   }

	/* Close down */
   BrPixelmapFree(depth_buffer);
   BrPixelmapFree(back_buffer);
   BrZbEnd();
   DOSGfxEnd();
   BrEnd();

   return 0;
}


