/*
 * Copyright (c) 1995 Argonaut Technologies Limited. All rights reserved.
 *
 * $Id:  $
 * $Locker:  $
 *
 */
#include <stddef.h>
#include <stdio.h>

#include "brender.h"
#include "dosio.h"

/*
 * The screen, offscreen buffer, and the depth buffer
 */
br_pixelmap *screen_buffer, *back_buffer;

/*
 * Pointer to primitive heap
 */
void *primitives;

/*
 * The actors in the world
 */
br_actor *observer, *world;
int main()
{
	br_actor *a;
	br_pixelmap *palette;
	int i;

	/*
	 * Setup renderer and screen
	 */
	BrBegin();
	screen_buffer = DOSGfxBegin(NULL);
	primitives = BrMemAllocate(10000,BR_MEMORY_APPLICATION);
	BrZsBegin(screen_buffer->type,primitives,10000);

	/*
	 * Setup CLUT (ignored in true-colour)
	 */
	palette = BrPixelmapLoad("std.pal");
	if(palette)
		DOSGfxPaletteSet(palette);

	/*
	 * Allocate other buffers
	 */
	back_buffer = BrPixelmapMatch(screen_buffer,BR_PMMATCH_OFFSCREEN);
	back_buffer->origin_x = back_buffer->width/2;
	back_buffer->origin_y = back_buffer->height/2;

	/*
	 * Build the a world
	 */
	world = BrActorAllocate(BR_ACTOR_NONE,NULL);
	observer = BrActorAdd(world,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
	BrLightEnable(BrActorAdd(world,BrActorAllocate(BR_ACTOR_LIGHT,NULL)));

	a = BrActorAdd(world,BrActorAllocate(BR_ACTOR_MODEL,NULL));
	a->t.type = BR_TRANSFORM_EULER;
	a->t.t.euler.e.order = BR_EULER_ZXY_S;
	BrVector3Set(&a->t.t.euler.t,BR_SCALAR(0.0),BR_SCALAR(0.0),BR_SCALAR(-5.0));

	/*
	 * Tumble the actor around
	 */
	for(i=0; i < 360; i++) {
		/*
		 * Clear the buffers
		 */
		BrPixelmapFill(back_buffer,0);

                BrZsSceneRender(world,observer,back_buffer);
		BrPixelmapDoubleBuffer(screen_buffer,back_buffer);

		a->t.t.euler.e.a = BR_ANGLE_DEG(i);
		a->t.t.euler.e.b = BR_ANGLE_DEG(i*2);
		a->t.t.euler.e.c = BR_ANGLE_DEG(i*3);
	}

	/*
	 * Close down
	 */
	BrPixelmapFree(back_buffer);

        BrZsEnd();
	DOSGfxEnd();
	BrEnd();

	return 0;
}

