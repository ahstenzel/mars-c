/*
 * vec2.h
 * Declaration for 2D vector of floats.
 */

#ifndef MARS_MATH_VEC2_H
#define MARS_MATH_VEC2_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "internal/platform.h"

 /* Define vec2 struct */
typedef struct {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float data[2];
#endif
} vec2;


/* vec2 functions */
vec2 vec2_make(float x, float y);

vec2 vec2_makez();

float vec2_getx(vec2* v);

float vec2_gety(vec2* v);

void vec2_setx(vec2* v, float x);

void vec2_sety(vec2* v, float y);

bool vec2_equal(vec2* a, vec2* b);

bool vec2_equalz(vec2* v);

vec2 vec2_add(vec2* a, vec2* b);

vec2 vec2_sub(vec2* a, vec2* b);

vec2 vec2_mul(vec2* a, vec2* b);

vec2 vec2_div(vec2* a, vec2* b);

float vec2_mag(vec2* v);

vec2 vec2_scale(vec2* a, float s);

vec2 vec2_norm(vec2* v);

float vec2_dist(vec2* a, vec2* b);

#endif
