/*
 * Copyright (c) 1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: model.h 2.8 1996/02/08 17:08:31 sam Exp $
 * $Locker:  $
 *
 * In-memory structures for models, both public and private areas
 */
#ifndef _MODEL_H_
#define _MODEL_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct br_vertex {
	br_vector3 p;	/* Point in model space		*/
	br_vector2 map;	/* Mapping coordinates		*/

	/*
	 * Colour for prelit models
	 */
	br_uint_8 index;
	br_uint_8 red;
	br_uint_8 grn;
	br_uint_8 blu;

	/*
	 * Private fields
	 */
	br_uint_16 _pad0;
	br_fvector3 n;	/* Surface normal at vertex	*/

} br_vertex;

typedef struct br_face {
	br_uint_16 vertices[3];		/* Vertices around face 				*/
	br_uint_16 smoothing;		/* Controls if shared edges are smooth	*/
	br_material *material;		/* Face material (or NULL) 				*/

	/*
	 * Colour for prelit models
	 */
	br_uint_8 index;
	br_uint_8 red;
	br_uint_8 grn;
	br_uint_8 blu;

	br_uint_8 flags;			/* Bits 0,1 and 2 denote internal edges	*/
	br_uint_8 _pad0;
	br_uint_32 _pad1;

	br_fvector3 n;				/* Plane equation of face				*/
	br_scalar d;
} br_face;

/*
 * Bits for face flags
 */
enum {
	BR_FACEF_COPLANAR_0 = 0x01,	/* The face adjoining edge 0 is coplanar with this face */
	BR_FACEF_COPLANAR_1 = 0x02,	/*         ""              1          ""                */
	BR_FACEF_COPLANAR_2 = 0x04	/*         ""              2          ""                */
};

/*
 * Callback function type for custom models
 */
struct br_actor;
struct br_model;
struct br_material;

typedef void BR_CALLBACK br_model_custom_cbfn(
				struct br_actor *actor,
				struct br_model *model,
				struct br_material *material,
				void *render_data,
				br_uint_8 style,
				int on_screen,
				br_matrix34 *model_to_view,
				br_matrix4 *model_to_screen);

typedef struct br_model {
	br_uint_32 _reserved;

	char *identifier;

	br_vertex *vertices;
	br_face *faces;

	br_uint_16 nvertices;
	br_uint_16 nfaces;

	/*
	 * Offset of model's pivot point (where it attaches to parent)
	 */
	br_vector3 pivot;

	/*
	 * Flags describing what is allowed in ModelPrepare()
	 */
	br_uint_16 flags;

	/*
	 * Application call
	 */
	br_model_custom_cbfn *custom;

	/*
	 * Application defined data - untouched by system
	 */
	void *user;

	/*
	 * Crease angle (used if MODF_CREASE is set)
	 */
	br_angle crease_angle;

	/*
	 * Bounding radius of model from origin
	 */
	br_scalar radius;

	/*
	 * Axis-aligned box that bounds model in model coords
	 */
	br_bounds bounds;

	/*
	 * Private fields
	 */
	void *prepared;
	void *stored;

} br_model;

/*
 * Bits for br_model->flags
 */
enum {
	BR_MODF_DONT_WELD		= 0x0001,	/* Vertices with same x,y,z cannot be merged	*/
	BR_MODF_KEEP_ORIGINAL	= 0x0002,	/* Obselete */
	BR_MODF_GENERATE_TAGS	= 0x0004,	/* Obselete */
	BR_MODF_QUICK_UPDATE	= 0x0010,	/* ModelUpdate is fast - but may produce slower models */

	BR_MODF_CUSTOM			= 0x0020,   /* Invoke custom callback for this model */
	BR_MODF_PREPREPARED		= 0x0040,	/* Model structure is pre-prepared - update performs no work */

	BR_MODF_UPDATEABLE		= 0x0080,	/* ModelUpdate can be used */

	BR_MODF_CREASE			= 0x0100,	/* Create creases in smoothing along edges if face<->face angle is g.t model->crease */
	BR_MODF_CUSTOM_NORMALS	= 0x0200,	/* Uses vertex normals from br_vertex structure */
	BR_MODF_CUSTOM_BOUNDS	= 0x0400,	/* Bounding box is already set up				*/
	BR_MODF_FACES_ONLY		= 0x0800,	/* Model will only be used to render faces (not edges or points) */

	_BR_MODF_RESERVED		= 0x8000
};

/*
 * Flags to BrModelUpdate()
 */
enum {
	BR_MODU_VERTEX_POSITIONS	= 0x0001,
	BR_MODU_VERTEX_COLOURS		= 0x0002,
	BR_MODU_VERTEX_MAPPING		= 0x0004,
	BR_MODU_VERTEX_NORMALS		= 0x0008,
	BR_MODU_FACE_MATERIALS		= 0x0010,
	BR_MODU_FACE_COLOURS		= 0x0020,
	BR_MODU_VERTICES			= 0x0040,
	BR_MODU_FACES				= 0x0080,
	BR_MODU_PIVOT				= 0x0100,
	BR_MODU_ALL					= 0x7FFF
};

/*
 * Backwards compatibility
 */
#define BR_MODU_NORMALS       	BR_MODU_VERTEX_POSITIONS
#define BR_MODU_EDGES		 	BR_MODU_FACES
#define BR_MODU_RADIUS		 	BR_MODU_VERTEX_POSITIONS
#define BR_MODU_GROUPS		 	BR_MODU_FACES
#define BR_MODU_BOUNDING_BOX	BR_MODU_VERTEX_POSITIONS
#define BR_MODU_MATERIALS      	BR_MODU_FACE_MATERIALS

/*
 * Values for BrModelApplyMap()
 */
enum br_apply_map_types {
	BR_APPLYMAP_PLANE,
	BR_APPLYMAP_SPHERE,
	BR_APPLYMAP_CYLINDER,
	BR_APPLYMAP_DISC,
	BR_APPLYMAP_NONE
};

/*
 * Axis values for BrModelFitMap()
 */
enum br_fitmap_axis {
	BR_FITMAP_PLUS_X,
	BR_FITMAP_PLUS_Y,
	BR_FITMAP_PLUS_Z,
	BR_FITMAP_MINUS_X,
	BR_FITMAP_MINUS_Y,
	BR_FITMAP_MINUS_Z
};

#ifdef __cplusplus
};
#endif
#endif

/* 
 * Local Variables:
 * tab-width: 4
 * End:
 */

