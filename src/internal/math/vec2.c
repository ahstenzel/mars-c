/*
 * vec2.c
 * Declaration for 2D vector of floats.
 */

#include "internal/math/vec2.h"

vec2 vec2_make(float x, float y) {
  vec2 v;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v.data[0] = x;
  v.data[1] = y;
#endif
  return v;
}

vec2 vec2_dup(float data[]) {
  vec2 v;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  memcpy(v.data, data, 2 * sizeof(float));
#endif
  return v;
}

vec2 vec2_makez() {
  vec2 v;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v.data[0] = 0.0f;
  v.data[1] = 0.0f;
#endif
  return v;
}

bool vec2_equal(vec2 a, vec2 b) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (a.data[0] == b.data[0] &&
          a.data[1] == b.data[1]);
#endif
}

bool vec2_equalz(vec2 v) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (v.data[0] == 0.0f &&
          v.data[1] == 0.0f);
#endif
}

vec2 vec2_add(vec2 a, vec2 b) {
  vec2 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] + b.data[0];
  r.data[1] = a.data[1] + b.data[1];
#endif
  return r;
}

vec2 vec2_sub(vec2 a, vec2 b) {
  vec2 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] - b.data[0];
  r.data[1] = a.data[1] - b.data[1];
#endif
  return r;
}

vec2 vec2_mul(vec2 a, vec2 b) {
  vec2 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] * b.data[0];
  r.data[1] = a.data[1] * b.data[1];
#endif
  return r;
}

vec2 vec2_div(vec2 a, vec2 b) {
  vec2 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] / b.data[0];
  r.data[1] = a.data[1] / b.data[1];
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
  float x = v.data[0];
  float y = v.data[1];
  return sqrtf(x * x + y * y);
#endif
}

vec2 vec2_scale(vec2 a, float s) {
  vec2 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] * s;
  r.data[1] = a.data[1] * s;
#endif
  return r;
}

vec2 vec2_norm(vec2 v) {
  vec2 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = v.data[0];
  float y = v.data[1];
  float mag = sqrt(x * x + y * y);
  r.data[0] = x / mag;
  r.data[1] = y / mag;

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
  float x = a.data[0] - b.data[0];
  float y = a.data[1] - b.data[1];
  double mag = sqrt(x * x + y * y);
  return (float)fabs(mag);
#endif
}
