/*
 * vec3.h
 * Declaration for 3D vector of floats.
 */

#ifndef MARS_MATH_VEC3_H
#define MARS_MATH_VEC3_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
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


/* vec3 functions */
vec3 vec3_make(float x, float y, float z);

vec3 vec3_makez();

float vec3_getx(vec3* v);

float vec3_gety(vec3* v);

float vec3_getz(vec3* v);

void vec3_setx(vec3* v, float x);

void vec3_sety(vec3* v, float y);

void vec3_setz(vec3* v, float z);

bool vec3_equal(vec3* a, vec3* b);

bool vec3_equalz(vec3* v);

vec3 vec3_add(vec3* a, vec3* b);

vec3 vec3_sub(vec3* a, vec3* b);

vec3 vec3_mul(vec3* a, vec3* b);

vec3 vec3_div(vec3* a, vec3* b);

float vec3_mag(vec3* v);

vec3 vec3_scale(vec3* a, float s);

vec3 vec3_norm(vec3* v);

float vec3_dist(vec3* a, vec3* b);

#endif
