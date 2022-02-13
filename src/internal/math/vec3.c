/*
 * vec3.c
 * Declaration for 3D vector of floats.
 */

#include "internal/math/vec3.h"

vec3 vec3_make(float x, float y, float z) {
  vec3 v = { 0 };
  vec3_setx(v, x);
  vec3_sety(v, y);
  vec3_setz(v, z);
  return v;
}

vec3 vec3_dup(float data[]) {
  vec3 v = { 0 };
  memcpy(v.data, data, 3 * sizeof(float));
  return v;
}

vec3 vec3_makez() {
  vec3 v = { 0 };
  vec3_setx(v, 0.0f);
  vec3_sety(v, 0.0f);
  vec3_setz(v, 0.0f);
  return v;
}

bool vec3_equal(vec3 a, vec3 b) {
  return (vec3_getx(a) == vec3_getx(b) &&
          vec3_gety(a) == vec3_gety(b) &&
          vec3_getz(a) == vec3_getz(b));
}

bool vec3_equalz(vec3 v) {
  return (vec3_getx(v) == 0.0f &&
          vec3_gety(v) == 0.0f &&
          vec3_getz(v) == 0.0f);
}

vec3 vec3_add(vec3 a, vec3 b) {
  vec3 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec3_setx(r, vec3_getx(a) + vec3_getx(b));
  vec3_sety(r, vec3_gety(a) + vec3_gety(b));
  vec3_setz(r, vec3_getz(a) + vec3_getz(b));
#endif
  return r;
}

vec3 vec3_sub(vec3 a, vec3 b) {
  vec3 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec3_setx(r, vec3_getx(a) - vec3_getx(b));
  vec3_sety(r, vec3_gety(a) - vec3_gety(b));
  vec3_setz(r, vec3_getz(a) - vec3_getz(b));
#endif
  return r;
}

vec3 vec3_mul(vec3 a, vec3 b) {
  vec3 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec3_setx(r, vec3_getx(a) * vec3_getx(b));
  vec3_sety(r, vec3_gety(a) * vec3_gety(b));
  vec3_setz(r, vec3_getz(a) * vec3_getz(b));
#endif
  return r;
}

vec3 vec3_div(vec3 a, vec3 b) {
  vec3 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec3_setx(r, vec3_getx(a) / vec3_getx(b));
  vec3_sety(r, vec3_gety(a) / vec3_gety(b));
  vec3_setz(r, vec3_getz(a) / vec3_getz(b));
#endif
  return r;
}

float vec3_dot(vec3 a, vec3 b) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (vec3_getx(a) * vec3_getx(b)) + (vec3_gety(a) * vec3_gety(b)) + (vec3_getz(a) * vec3_getz(b));
#endif
}

vec3 vec3_cross(vec3 a, vec3 b) {
  vec3 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec3_setx(r, (vec3_gety(a) * vec3_getz(b)) - (vec3_getz(a) * vec3_gety(b)));
  vec3_sety(r, (vec3_getz(a) * vec3_getx(b)) - (vec3_getx(a) * vec3_getz(b)));
  vec3_setz(r, (vec3_getx(a) * vec3_gety(b)) - (vec3_gety(a) * vec3_getx(b)));
#endif
  return r;
}

float vec3_mag(vec3 v) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = vec3_getx(v);
  float y = vec3_gety(v);
  float z = vec3_getz(v);
  return sqrtf(x * x + y * y + z * z);
#endif
}

vec3 vec3_scale(vec3 a, float s) {
  vec3 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  vec3_setx(r, vec3_getx(a) * s);
  vec3_sety(r, vec3_gety(a) * s);
  vec3_setz(r, vec3_getz(a) * s);
#endif
  return r;
}

vec3 vec3_norm(vec3 v) {
  vec3 r = { 0 };
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = vec3_getx(v);
  float y = vec3_gety(v);
  float z = vec3_getz(v);
  float mag = sqrtf(x * x + y * y + z * z);
  vec3_setx(v, x / mag);
  vec3_sety(v, y / mag);
  vec3_setz(v, z / mag);
#endif
  return r;
}

float vec3_dist(vec3 a, vec3 b) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = vec3_getx(a) - vec3_getx(b);
  float y = vec3_gety(a) - vec3_gety(b);
  float z = vec3_getz(a) - vec3_getz(b);
  float mag = sqrtf(x * x + y * y + z * z);
  return fabs(mag);
#endif
}
