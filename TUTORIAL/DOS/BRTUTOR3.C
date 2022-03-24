/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 * Program to Display Planet, Moon, Satellite animation
 */
#include <stddef.h>
#include <stdio.h>
#include "brender.h"
#include "dosio.h"


int main()
{

   br_pixelmap *screen_buffer, *back_buffer, *depth_buffer, *palette;
   br_actor *universe, *observer, *light, *planet, *moon, *satellite;
   int i;
   br_camera *camera_data;

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
	 
/************************* Build th World Database ************************/
	
/*Load Root Actor*/
	universe = BrActorAllocate(BR_ACTOR_NONE,NULL);
	
/*Load and Enable Default Light Source*/       
	light = BrActorAdd(universe,BrActorAllocate(BR_ACTOR_LIGHT,NULL));
	BrLightEnable(light);

/*Load and Position Camera*/        
	observer = BrActorAdd(universe,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	observer->t.type = BR_TRANSFORM_MATRIX34;        
	BrMatrix34Translate(&observer->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(6.0));
	camera_data = (br_camera *)observer->type_data;
	camera_data->yon_z = BR_SCALAR(50);

/*Load Planet Model*/
	planet = BrActorAdd(universe,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	planet->model = BrModelLoad("sph16.dat");
	BrModelAdd(planet->model);

/*Load and Position Moon Model*/
	moon = BrActorAdd(planet,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	moon->model = BrModelLoad("sph8.dat");
	BrModelAdd(moon->model);
	moon->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34Scale(&moon->t.t.mat,BR_SCALAR(0.5),BR_SCALAR(0.5),BR_SCALAR(0.5));
	BrMatrix34PostTranslate(&moon->t.t.mat,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(2.0));
	
/*Load and Position Satellite Model*/
	satellite = BrActorAdd(moon,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	satellite->model = BrModelLoad("sph8.dat");
	BrModelAdd(satellite->model);
	satellite->t.type = BR_TRANSFORM_MATRIX34;
	BrMatrix34Scale(&satellite->t.t.mat,BR_SCALAR(0.25),BR_SCALAR(0.25),BR_SCALAR(0.25));
	BrMatrix34PostTranslate(&satellite->t.t.mat,BR_SCALAR(1.5),BR_SCALAR(0.0),BR_SCALAR(0.0));   
	
/******************************** Animation Loop ***************************/                                                              
	
	for(i=0;i<1000;i++) {
		BrPixelmapFill(back_buffer,0);
		BrPixelmapFill(depth_buffer,0xFFFFFFFF);
		BrZbSceneRender(universe,observer,back_buffer,depth_buffer);
		BrPixelmapDoubleBuffer(screen_buffer,back_buffer);
		
		BrMatrix34PreRotateY(&planet->t.t.mat,BR_ANGLE_DEG(1.0));                
		BrMatrix34PreRotateY(&satellite->t.t.mat,BR_ANGLE_DEG(4.0));  
		BrMatrix34PreRotateZ(&moon->t.t.mat,BR_ANGLE_DEG(1.5));  
		
		BrMatrix34PostRotateZ(&satellite->t.t.mat,BR_ANGLE_DEG(-2.5));
		BrMatrix34PostRotateY(&moon->t.t.mat,BR_ANGLE_DEG(-2.0)); 
	}

/* Close down*/
	BrPixelmapFree(depth_buffer);
	BrPixelmapFree(back_buffer);
	BrZbEnd();
	DOSGfxEnd();
	BrEnd();

	return 0;
}


