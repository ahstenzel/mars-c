/*
 * mat3.h
 * Declaration for 3x3 matrix of floats in column-major order.
 */

#ifndef MARS_MATH_MAT3_H
#define MARS_MATH_MAT3_H

#include <string.h>
#include "internal/platform.h"
#include "internal/math/vec3.h"
#include "internal/math/mat2.h"

 /* Define mat3 struct */
typedef struct {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float data[9];
#endif
} mat3;

#define mat3_get(m, r, c) (m.data[(c * 3) + r])
#define mat3_set(m, v, r, c) (m.data[(c * 3) + r] = v)

/* mat3 functions */
mat3 mat3_make(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

mat3 mat3_dup(float data[]);

mat3 mat3_makeid();

bool mat3_equal(mat3 a, mat3 b);

bool mat3_equalid(mat3 m);

mat3 mat3_add(mat3 a, mat3 b);

mat3 mat3_sub(mat3 a, mat3 b);

mat3 mat3_scale(mat3 m, float s);

mat3 mat3_mul(mat3 a, mat3 b);

vec3 mat3_vec3_mul(mat3 m, vec3 v);

mat3 mat3_transpose(mat3 m);

float mat3_det(mat3 m);

mat2 mat3_submatrix(mat3 m, unsigned int row, unsigned int col);

#endif
