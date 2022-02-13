/*
 * vec4.c
 * Declaration for 4D vector of floats.
 */

#include "internal/math/vec4.h"

vec4 vec4_make(float x, float y, float z, float w) {
  vec4 v = { 0 };
  vec4_setx(v, x);
  vec4_sety(v, y);
  vec4_setz(v, z);
  vec4_setw(v, w);
  return v;
}

vec4 vec4_dup(float data[]) {
  vec4 v = { 0 };
  memcpy(v.data, data, 4 * sizeof(float));
  return v;
}

vec4 vec4_makez() {
  vec4 v = { 0 };
  vec4_setx(v, 0.0f);
  vec4_sety(v, 0.0f);
  vec4_setz(v, 0.0f);
  vec4_setw(v, 0.0f);
  return v;
}

bool vec4_equal(vec4 a, vec4 b) {
  return (vec4_getx(a) == vec4_getx(b) &&
          vec4_gety(a) == vec4_gety(b) &&
          vec4_getz(a) == vec4_getz(b) &&
          vec4_getw(a) == vec4_getw(b));
}

bool vec4_equalz(vec4 v) {
  return (vec4_getx(v) == 0.0f &&
          vec4_gety(v) == 0.0f &&
          vec4_getw(v) == 0.0f &&
          vec4_getz(v) == 0.0f);
}

vec4 vec4_add(vec4 a, vec4 b) {
  vec4 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec4_setx(r, vec4_getx(a) + vec4_getx(b));
  vec4_sety(r, vec4_gety(a) + vec4_gety(b));
  vec4_setz(r, vec4_getz(a) + vec4_getz(b));
  vec4_setw(r, vec4_getw(a) + vec4_getw(b));
#endif
  return r;
}

vec4 vec4_sub(vec4 a, vec4 b) {
  vec4 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec4_setx(r, vec4_getx(a) - vec4_getx(b));
  vec4_sety(r, vec4_gety(a) - vec4_gety(b));
  vec4_setz(r, vec4_getz(a) - vec4_getz(b));
  vec4_setw(r, vec4_getw(a) - vec4_getw(b));
#endif
  return r;
}

vec4 vec4_mul(vec4 a, vec4 b) {
  vec4 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec4_setx(r, vec4_getx(a) * vec4_getx(b));
  vec4_sety(r, vec4_gety(a) * vec4_gety(b));
  vec4_setz(r, vec4_getz(a) * vec4_getz(b));
  vec4_setw(r, vec4_getw(a) * vec4_getw(b));
#endif
  return r;
}

vec4 vec4_div(vec4 a, vec4 b) {
  vec4 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec4_setx(r, vec4_getx(a) / vec4_getx(b));
  vec4_sety(r, vec4_gety(a) / vec4_gety(b));
  vec4_setz(r, vec4_getz(a) / vec4_getz(b));
  vec4_setw(r, vec4_getw(a) / vec4_getw(b));
#endif
  return r;
}

float vec4_mag(vec4 v) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = vec4_getx(v);
  float y = vec4_gety(v);
  float z = vec4_getz(v);
  float w = vec4_getw(v);
  return sqrtf(x * x + y * y + z * z + w * w);
#endif
}

vec4 vec4_scale(vec4 a, float s) {
  vec4 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec4_setx(r, vec4_getx(a) * s);
  vec4_sety(r, vec4_gety(a) * s);
  vec4_setz(r, vec4_getz(a) * s);
  vec4_setw(r, vec4_getw(a) * s);
#endif
  return r;
}

vec4 vec4_norm(vec4 v) {
  vec4 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = vec4_getx(v);
  float y = vec4_gety(v);
  float z = vec4_getz(v);
  float w = vec4_getw(v);
  float mag = sqrtf(x * x + y * y + z * z + w * w);
  vec4_setx(v, x / mag);
  vec4_sety(v, y / mag);
  vec4_setz(v, z / mag);
  vec4_setz(v, w / mag);
#endif
  return r;
}

float vec4_dist(vec4 a, vec4 b) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = vec4_getx(a) - vec4_getx(b);
  float y = vec4_gety(a) - vec4_gety(b);
  float z = vec4_getz(a) - vec4_getz(b);
  float w = vec4_getw(a) - vec4_getw(b);
  float mag = sqrtf(x * x + y * y + z * z + w * w);
  return fabs(mag);
#endif
}
