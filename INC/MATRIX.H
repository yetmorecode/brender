/*
 * Copyright (c) 1992,1993-1995 by Argonaut Technologies Limited. All rights reserved.
 *
 * $Id: matrix.h 2.1 1995/08/02 20:06:05 sam Exp $
 * $Locker:  $
 *
 * Structrures describing matrices
 */
#ifndef _MATRIX_H_
#define _MATRIX_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct br_matrix23 {
	br_scalar m[3][2];
} br_matrix23;

typedef struct br_matrix23_f {
	br_float m[3][2];
} br_matrix23_f;

typedef struct br_matrix23_x {
	br_fixed_ls m[3][2];
} br_matrix23_x;

typedef struct br_matrix34 {
	br_scalar m[4][3];
} br_matrix34;

typedef struct br_matrix34_f {
	br_float m[4][3];
} br_matrix34_f;

typedef struct br_matrix34_x {
	br_fixed_ls m[4][3];
} br_matrix34_x;

typedef struct br_matrix4 {
	br_scalar m[4][4];
} br_matrix4;

typedef struct br_matrix4_f {
	br_float m[4][4];
} br_matrix4_f;

typedef struct br_matrix4_x {
	br_fixed_ls m[4][4];
} br_matrix4_x;

#ifdef __cplusplus
};
#endif
#endif

