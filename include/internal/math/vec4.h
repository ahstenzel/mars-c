/*
 * vec4.h
 * Declaration for 4D vector of floats.
 */

#ifndef MARS_MATH_VEC4_H
#define MARS_MATH_VEC4_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
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


/* vec4 functions */
vec4 vec4_make(float x, float y, float z, float w);

vec4 vec4_makez();

float vec4_getx(vec4* v);

float vec4_gety(vec4* v);

float vec4_getw(vec4* v);

float vec4_getz(vec4* v);

void vec4_setx(vec4* v, float x);

void vec4_sety(vec4* v, float y);

void vec4_setz(vec4* v, float z);

void vec4_setw(vec4* v, float z);

bool vec4_equal(vec4* a, vec4* b);

bool vec4_equalz(vec4* v);

vec4 vec4_add(vec4* a, vec4* b);

vec4 vec4_sub(vec4* a, vec4* b);

vec4 vec4_mul(vec4* a, vec4* b);

vec4 vec4_div(vec4* a, vec4* b);

float vec4_mag(vec4* v);

vec4 vec4_scale(vec4* a, float s);

vec4 vec4_norm(vec4* v);

float vec4_dist(vec4* a, vec4* b);

#endif
