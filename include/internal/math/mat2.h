/*
 * mat2.h
 * Declaration for 2x2 matrix of floats in column-major order.
 */

#ifndef MARS_MATH_mat2_H
#define MARS_MATH_mat2_H

#include "internal/platform.h"
#include "internal/math/vec2.h"

 /* Define mat2 struct */
typedef struct {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float data[4];
#endif
} mat2;


/* mat2 functions */
mat2 mat2_make(float x1, float y1, float x2, float y2);

mat2 mat2_makeid();

bool mat2_equal(mat2* a, mat2* b);

bool mat2_equalid(mat2* m);

float mat2_get(unsigned int col, unsigned int row, mat2* m);

mat2 mat2_add(mat2* a, mat2* b);

mat2 mat2_sub(mat2* a, mat2* b);

mat2 mat2_scale(mat2* m, float s);

mat2 mat2_mul(mat2* a, mat2* b);

vec2 mat2_vec2_mul(mat2* m, vec2* v);

mat2 mat2_transpose(mat2* m);

float mat2_det(mat2* m);

#endif
