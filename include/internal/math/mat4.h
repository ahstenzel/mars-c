/*
 * mat4.h
 * Declaration for 4x4 matrix of floats in column-major order.
 */

#ifndef MARS_MATH_MAT4_H
#define MARS_MATH_MAT4_H

#include <string.h>
#include "internal/platform.h"
#include "internal/math/vec4.h"
#include "internal/math/mat3.h"

 /* Define mat4 struct */
typedef struct {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float data[16];
#endif
} mat4;

#define mat4_get(m, r, c) (m.data[(c * 4) + r])
#define mat4_set(m, v, r, c) (m.data[(c * 4) + r] = v)

/* mat4 functions */
mat4 mat4_make(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4);

mat4 mat4_dup(float data[]);

mat4 mat4_makeid();

bool mat4_equal(mat4 a, mat4 b);

bool mat4_equalid(mat4 m);

mat4 mat4_add(mat4 a, mat4 b);

mat4 mat4_sub(mat4 a, mat4 b);

mat4 mat4_scale(mat4 m, float s);

mat4 mat4_mul(mat4 a, mat4 b);

vec4 mat4_vec4_mul(mat4 m, vec4 v);

mat4 mat4_transpose(mat4 m);

float mat4_det(mat4 m);

mat3 mat4_submatrix(mat4 m, unsigned int row, unsigned int col);

#endif
