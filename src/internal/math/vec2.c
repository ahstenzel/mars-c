/*
 * vec2.c
 * Declaration for 2D vector of floats.
 */

#include "internal/math/vec2.h"

vec2 vec2_make(float x, float y) {
  vec2 v = { 0 };
  vec2_setx(v, x);
  vec2_sety(v, y);
  return v;
}

vec2 vec2_dup(float data[]) {
  vec2 v = { 0 };
  memcpy(v.data, data, 2 * sizeof(float));
  return v;
}

vec2 vec2_makez() {
  vec2 v = { 0 };
  vec2_setx(v, 0.0f);
  vec2_sety(v, 0.0f);
  return v;
}

bool vec2_equal(vec2 a, vec2 b) {
  return (vec2_getx(a) == vec2_getx(b) &&
          vec2_gety(a) == vec2_gety(b));
}

bool vec2_equalz(vec2 v) {
  return (vec2_getx(v) == 0.0f &&
          vec2_gety(v) == 0.0f);
}

vec2 vec2_add(vec2 a, vec2 b) {
  vec2 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec2_setx(r, vec2_getx(a) + vec2_getx(b));
  vec2_sety(r, vec2_gety(a) + vec2_gety(b));
#endif
  return r;
}

vec2 vec2_sub(vec2 a, vec2 b) {
  vec2 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec2_setx(r, vec2_getx(a) - vec2_getx(b));
  vec2_sety(r, vec2_gety(a) - vec2_gety(b));
#endif
  return r;
}

vec2 vec2_mul(vec2 a, vec2 b) {
  vec2 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec2_setx(r, vec2_getx(a) * vec2_getx(b));
  vec2_sety(r, vec2_gety(a) * vec2_gety(b));
#endif
  return r;
}

vec2 vec2_div(vec2 a, vec2 b) {
  vec2 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec2_setx(r, vec2_getx(a) / vec2_getx(b));
  vec2_sety(r, vec2_gety(a) / vec2_gety(b));
#endif
  return r;
}

float vec2_mag(vec2 v) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = vec2_getx(v);
  float y = vec2_gety(v);
  return sqrtf(x * x + y * y);
#endif
}

vec2 vec2_scale(vec2 a, float s) {
  vec2 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec2_setx(r, vec2_getx(a) * s);
  vec2_sety(r, vec2_gety(a) * s);
#endif
  return r;
}

vec2 vec2_norm(vec2 v) {
  vec2 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = vec2_getx(v);
  float y = vec2_gety(v);
  float mag = sqrtf(x * x + y * y);
  vec2_setx(v, x / mag);
  vec2_sety(v, y / mag);
#endif
  return r;
}

float vec2_dist(vec2 a, vec2 b) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = vec2_getx(a) - vec2_getx(b);
  float y = vec2_gety(a) - vec2_gety(b);
  float mag = sqrtf(x * x + y * y);
  return fabs(mag);
#endif
}
