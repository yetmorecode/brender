#include <stdio.h>
#include <string.h>
#include <env.h>
#include <stdlib.h>
#include <dos.h>

#include "brender.h"
#include "dosio.h"
#include "fmt.h"

#define S0 BR_SCALAR(0.0)
#define S1 BR_SCALAR(1.0)

main(int argc,char *argv[]) {
    br_pixelmap *screen,*back_screen,*depth_buffer,*palette,*shade,*texture;
    br_pixelmap *texture2;
    br_actor *root,*camera,*camera2,*light,*object,*object1;
    int n,new_cm;
    br_matrix34 rotation;
    long x,y;
    unsigned long b;
    br_material *material,*material2;

    BrBegin();
    DOSKeyBegin();

    setenv("BRENDER_DOSGFX","MCGA,W:320,H:200,B:8,C:0,M:0",0);
    screen = DOSGfxBegin(NULL);

    back_screen = BrPixelmapMatch(screen,BR_PMMATCH_OFFSCREEN);
    depth_buffer = BrPixelmapMatch(back_screen,BR_PMMATCH_DEPTH_16);
    BrZbBegin(back_screen->type,depth_buffer->type);

    palette = BrPixelmapLoad("std.pal");
    DOSGfxPaletteSet(palette);

    shade = BrPixelmapLoad("shade.tab");
    shade->identifier = "shade";
    BrTableAdd(shade);

    texture = BrPixelmapLoad("rosewood.pix");
    texture->identifier = "grain";
    BrMapAdd(texture);

    texture2 = BrPixelmapLoad("marble.pix");
    texture2->identifier = "veins";
    BrMapAdd(texture2);

    material = BrMaterialAllocate(NULL);
    material->identifier = "wood";
    material->colour_map = BrMapFind("grain");
    material->ka = BR_SCALAR(0.33);
    material->kd = BR_SCALAR(0.33);
    material->ks = BR_SCALAR(0.33);
    material->power = BR_SCALAR(20);
    material->flags = BR_MATF_PERSPECTIVE | BR_MATF_GOURAUD | BR_MATF_LIGHT;
    material->map_transform.m[0][0] = S1;
    material->map_transform.m[1][1] = S1;
    material->map_transform.m[0][1] = S0;
    material->map_transform.m[1][0] = S0;
    material->map_transform.m[2][0] = S0;
    material->map_transform.m[2][1] = S0;
    material->index_base = 0;
    material->index_range = 63;
    material->index_shade = BrTableFind("shade");
    BrMaterialAdd(material);

    material2 = BrMaterialAllocate(NULL);
    material2->identifier = "marble";
    material2->colour_map = BrMapFind("veins");
    material2->ka = BR_SCALAR(0.33);
    material2->kd = BR_SCALAR(0.33);
    material2->ks = BR_SCALAR(0.33);
    material2->power = BR_SCALAR(20);
    material2->flags = BR_MATF_PERSPECTIVE | BR_MATF_GOURAUD | BR_MATF_LIGHT;
    material2->map_transform.m[0][0] = S1;
    material2->map_transform.m[1][1] = S1;
    material2->map_transform.m[0][1] = S0;
    material2->map_transform.m[1][0] = S0;
    material2->map_transform.m[2][0] = S0;
    material2->map_transform.m[2][1] = S0;
    material2->index_base = 0;
    material2->index_range = 63;
    material2->index_shade = BrTableFind("shade");
    BrMaterialAdd(material2);

    root = BrActorAllocate(BR_ACTOR_NONE,NULL);

    camera = BrActorAdd(root,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
    camera->identifier = "camera";
    ((br_camera *)camera->type_data)->type = BR_CAMERA_PERSPECTIVE;
    ((br_camera *)camera->type_data)->field_of_view = BR_ANGLE_DEG(55.0);
    ((br_camera *)camera->type_data)->hither_z = BR_SCALAR(0.01); 
    ((br_camera *)camera->type_data)->yon_z = BR_SCALAR(100.0); 
    ((br_camera *)camera->type_data)->aspect = BR_SCALAR(1.6); 
    BrMatrix34Translate(&camera->t.t.mat,S0,S0,S1+S1);

    camera2 = BrActorAdd(root,BrActorAllocate(BR_ACTOR_CAMERA,NULL));
    camera2->identifier = "camera2";
    ((br_camera *)camera2->type_data)->type = BR_CAMERA_PERSPECTIVE;
    ((br_camera *)camera2->type_data)->field_of_view = BR_ANGLE_DEG(55.0);
    ((br_camera *)camera2->type_data)->hither_z = BR_SCALAR(0.01); 
    ((br_camera *)camera2->type_data)->yon_z = BR_SCALAR(100.0); 
    ((br_camera *)camera2->type_data)->aspect = BR_SCALAR(1.6); 
    BrMatrix34Translate(&camera2->t.t.mat,S0,S0,S1+S1+S1+S1+S1);

    light = BrActorAdd(root,BrActorAllocate(BR_ACTOR_LIGHT,NULL));
    light->identifier = "light";
    ((br_light *)light->type_data)->type = BR_LIGHT_DIRECT;
    ((br_light *)light->type_data)->attenuation_c = BR_SCALAR(1.0);
    BrLightEnable(light);

    object = BrActorAdd(root,BrActorAllocate(BR_ACTOR_MODEL,NULL));
    object->model = BrModelLoad("venus.dat");
    if (object->model==NULL)
	BR_ERROR0("Couldn't load venus.dat");
    BrModelAdd(object->model);
    object->material = BrMaterialFind("wood");
    BrMatrix34RotateX(&object->t.t.mat,BR_ANGLE_DEG(-90));
    BrMatrix34PostTranslate(&object->t.t.mat,
	BR_SCALAR(-1.0),BR_SCALAR(0.0),BR_SCALAR(-1.0));

    object1 = BrActorAdd(root,BrActorAllocate(BR_ACTOR_MODEL,NULL));
    object1->model = BrModelLoad("torus.dat");
    if (object1->model==NULL)
	BR_ERROR0("Couldn't load torus.dat");
    BrModelAdd(object1->model);
    object1->material = BrMaterialFind("marble");
    BrMatrix34PostTranslate(&object1->t.t.mat,
	BR_SCALAR(1.0),BR_SCALAR(0.0),BR_SCALAR(-1.0));

    BrPixelmapFill(back_screen,0);

    BrPixelmapFill(depth_buffer,0xffffffff);

    BrZbSceneRender(root,camera,back_screen,depth_buffer);

    back_screen = BrPixelmapDoubleBuffer(screen,back_screen);

    while (!DOSKeyTest(SC_ESC,0,0));

    BrActorSave("example.act",root);
    BrMaterialSave("example.mat",material);
    BrMaterialSave("example2.mat",material2);
    BrPixelmapSave("example.pix",texture);
    BrPixelmapSave("example2.pix",texture2);
    BrPixelmapSave("example.tab",shade);
    BrPixelmapSave("example.pal",palette);

    DOSKeyEnd();
    DOSGfxEnd();
    BrZbEnd();
    BrEnd();
}
