/*
 * vec4.h
 * Declaration for 4D vector of floats.
 */

#ifndef MARS_MATH_VEC4_H
#define MARS_MATH_VEC4_H

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "internal/platform.h"

 /* Define vec4 struct */
typedef struct {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float data[4];
#endif
} vec4;

#define vec4_get(v, i) (v.data[i])
#define vec4_set(v, i, f) (v.data[i] = f)

#define vec4_getx(v) (v.data[0])
#define vec4_setx(v, f) (v.data[0] = f)

#define vec4_gety(v) (v.data[1])
#define vec4_sety(v, f) (v.data[1] = f)

#define vec4_getz(v) (v.data[2])
#define vec4_setz(v, f) (v.data[2] = f)

#define vec4_getw(v) (v.data[3])
#define vec4_setw(v, f) (v.data[3] = f)

/* vec4 functions */
vec4 vec4_make(float x, float y, float z, float w);

vec4 vec4_dup(float data[]);

vec4 vec4_makez();

bool vec4_equal(vec4 a, vec4 b);

bool vec4_equalz(vec4 v);

vec4 vec4_add(vec4 a, vec4 b);

vec4 vec4_sub(vec4 a, vec4 b);

vec4 vec4_mul(vec4 a, vec4 b);

vec4 vec4_div(vec4 a, vec4 b);

float vec4_mag(vec4 v);

vec4 vec4_scale(vec4 a, float s);

vec4 vec4_norm(vec4 v);

float vec4_dist(vec4 a, vec4 b);

#endif
