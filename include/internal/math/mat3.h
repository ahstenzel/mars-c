/*
 * mat3.h
 * Declaration for 3x3 matrix of floats in column-major order.
 */

#ifndef CAM_LINEAR_mat3_H
#define CAM_LINEAR_mat3_H

#include "internal/platform.h"
#include "internal/math/vec3.h"

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


/* mat3 functions */
mat3 mat3_make(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3);

mat3 mat3_makeid();

bool mat3_equal(mat3* a, mat3* b);

bool mat3_equalid(mat3* m);

float mat3_get(unsigned int col, unsigned int row, mat3* m);

mat3 mat3_add(mat3* a, mat3* b);

mat3 mat3_sub(mat3* a, mat3* b);

mat3 mat3_scale(mat3* m, float s);

mat3 mat3_mul(mat3* a, mat3* b);

vec2 mat3_vec3_mul(mat3* m, vec3* v);

mat3 mat3_transpose(mat3* m);

float mat3_det(mat3* m);

#endif
