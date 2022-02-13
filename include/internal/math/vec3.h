/*
 * vec3.h
 * Declaration for 3D vector of floats.
 */

#ifndef MARS_MATH_VEC3_H
#define MARS_MATH_VEC3_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "internal/platform.h"

 /* Define vec3 struct */
typedef struct {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float data[3];
#endif
} vec3;

#define vec3_get(v, i) (v.data[i])
#define vec3_set(v, i, f) (v.data[i] = f)

#define vec3_getx(v) (v.data[0])
#define vec3_setx(v, f) (v.data[0] = f)

#define vec3_gety(v) (v.data[1])
#define vec3_sety(v, f) (v.data[1] = f)

#define vec3_getz(v) (v.data[2])
#define vec3_setz(v, f) (v.data[2] = f)

/* vec3 functions */
vec3 vec3_make(float x, float y, float z);

vec3 vec3_dup(float data[]);

vec3 vec3_makez();

bool vec3_equal(vec3 a, vec3 b);

bool vec3_equalz(vec3 v);

vec3 vec3_add(vec3 a, vec3 b);

vec3 vec3_sub(vec3 a, vec3 b);

vec3 vec3_mul(vec3 a, vec3 b);

vec3 vec3_div(vec3 a, vec3 b);

float vec3_dot(vec3 a, vec3 b);

vec3 vec3_cross(vec3 a, vec3 b);

float vec3_mag(vec3 v);

vec3 vec3_scale(vec3 a, float s);

vec3 vec3_norm(vec3 v);

float vec3_dist(vec3 a, vec3 b);

#endif
