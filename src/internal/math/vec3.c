/*
 * vec3.c
 * Declaration for 3D vector of floats.
 */

#include "internal/math/vec3.h"

vec3 vec3_make(float x, float y, float z) {
  vec3 v;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v.data[0] = x;
  v.data[1] = y;
  v.data[2] = z;
#endif
  return v;
}

vec3 vec3_dup(float data[]) {
  vec3 v;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  memcpy(v.data, data, 3 * sizeof(float));
#endif
  return v;
}

vec3 vec3_makez() {
  vec3 v;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v.data[0] = 0.0f;
  v.data[1] = 0.0f;
  v.data[2] = 0.0f;
#endif
  return v;
}

bool vec3_equal(vec3 a, vec3 b) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (a.data[0] == b.data[0] &&
          a.data[1] == b.data[1] &&
          a.data[2] == b.data[2]);
#endif
}

bool vec3_equalz(vec3 v) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (v.data[0] == 0.0f &&
          v.data[1] == 0.0f &&
          v.data[2] == 0.0f);
#endif
}

vec3 vec3_add(vec3 a, vec3 b) {
  vec3 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] + b.data[0];
  r.data[1] = a.data[1] + b.data[1];
  r.data[2] = a.data[2] + b.data[2];
#endif
  return r;
}

vec3 vec3_sub(vec3 a, vec3 b) {
  vec3 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] - b.data[0];
  r.data[1] = a.data[1] - b.data[1];
  r.data[2] = a.data[2] - b.data[2];
#endif
  return r;
}

vec3 vec3_mul(vec3 a, vec3 b) {
  vec3 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] * b.data[0];
  r.data[1] = a.data[1] * b.data[1];
  r.data[2] = a.data[2] * b.data[2];
#endif
  return r;
}

vec3 vec3_div(vec3 a, vec3 b) {
  vec3 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] / b.data[0];
  r.data[1] = a.data[1] / b.data[1];
  r.data[2] = a.data[2] / b.data[2];
#endif
  return r;
}

float vec3_dot(vec3 a, vec3 b) {
  return (vec3_getx(a) * vec3_getx(b)) + (vec3_gety(a) * vec3_gety(b)) + (vec3_getz(a) * vec3_getz(b));
}

vec3 vec3_cross(vec3 a, vec3 b) {
  vec3 r;
  vec3_setx(r, (vec3_gety(a) * vec3_getz(b)) - (vec3_getz(a) * vec3_gety(b)));
  vec3_sety(r, (vec3_getz(a) * vec3_getx(b)) - (vec3_getx(a) * vec3_getz(b)));
  vec3_setz(r, (vec3_getx(a) * vec3_gety(b)) - (vec3_gety(a) * vec3_getx(b)));
  return r;
}

float vec3_mag(vec3 v) {
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = v.data[0];
  float y = v.data[1];
  float z = v.data[2];
  return sqrtf(x * x + y * y + z * z);
#endif
}

vec3 vec3_scale(vec3 a, float s) {
  vec3 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a.data[0] * s;
  r.data[1] = a.data[1] * s;
  r.data[2] = a.data[2] * s;
#endif
  return r;
}

vec3 vec3_norm(vec3 v) {
  vec3 r;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = v.data[0];
  float y = v.data[1];
  float z = v.data[2];
  float mag = sqrt(x * x + y * y + z * z);
  r.data[0] = x / mag;
  r.data[1] = y / mag;
  r.data[2] = z / mag;

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
  float x = a.data[0] - b.data[0];
  float y = a.data[1] - b.data[1];
  float z = a.data[2] - b.data[2];
  double mag = sqrt(x * x + y * y + z * z);
  return (float)fabs(mag);
#endif
}
