/*
 * vec4.c
 * Declaration for 4D vector of floats.
 */

#include "internal/math/vec4.h"

vec4 vec4_make(float x, float y, float z, float w) {
  vec4 v;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v.data[0] = x;
  v.data[1] = y;
  v.data[2] = z;
  v.data[3] = w;
#endif
  return v;
}

vec4 vec4_makez() {
  vec4 v;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v.data[0] = 0.0f;
  v.data[1] = 0.0f;
  v.data[2] = 0.0f;
  v.data[3] = 0.0f;
#endif
  return v;
}

float vec4_getx(vec4* v) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return v->data[0];
#endif
}

float vec4_gety(vec4* v) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return v->data[1];
#endif
}

float vec4_getz(vec4* v) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return v->data[2];
#endif
}

float vec4_getw(vec4* v) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return v->data[3];
#endif
}

void vec4_setx(vec4* v, float x) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v->data[0] = x;
#endif
}

void vec4_sety(vec4* v, float y) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v->data[1] = y;
#endif
}

void vec4_setz(vec4* v, float z) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v->data[2] = z;
#endif
}

void vec4_setw(vec4* v, float w) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  v->data[3] = w;
#endif
}

bool vec4_equal(vec4* a, vec4* b) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
  return mask == 0xF;
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (a->data[0] == b->data[0] &&
          a->data[1] == b->data[1] &&
          a->data[2] == b->data[2] &&
          a->data[3] == b->data[3]);
#endif
}

bool vec4_equalz(vec4* v) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
  return mask == 0xF;
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (v->data[0] == 0.0f &&
          v->data[1] == 0.0f &&
          v->data[2] == 0.0f &&
          v->data[3] == 0.0f);
#endif
}

vec4 vec4_add(vec4* a, vec4* b) {
  vec4 r;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a->data[0] + b->data[0];
  r.data[1] = a->data[1] + b->data[1];
  r.data[2] = a->data[2] + b->data[2];
  r.data[3] = a->data[3] + b->data[3];
#endif
  return r;
}

vec4 vec4_sub(vec4* a, vec4* b) {
  vec4 r;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a->data[0] - b->data[0];
  r.data[1] = a->data[1] - b->data[1];
  r.data[2] = a->data[2] - b->data[2];
  r.data[3] = a->data[3] - b->data[3];
#endif
  return r;
}

vec4 vec4_mul(vec4* a, vec4* b) {
  vec4 r;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a->data[0] * b->data[0];
  r.data[1] = a->data[1] * b->data[1];
  r.data[2] = a->data[2] * b->data[2];
  r.data[3] = a->data[3] * b->data[3];
#endif
  return r;
}

vec4 vec4_div(vec4* a, vec4* b) {
  vec4 r;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a->data[0] / b->data[0];
  r.data[1] = a->data[1] / b->data[1];
  r.data[2] = a->data[2] / b->data[2];
  r.data[3] = a->data[3] / b->data[3];
#endif
  return r;
}

float vec4_mag(vec4* v) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = v->data[0];
  float y = v->data[1];
  float z = v->data[2];
  float w = v->data[3];
  return (float)sqrt(x * x + y * y + z * z + w * w);
#endif
}

vec4 vec4_scale(vec4* a, float s) {
  vec4 r;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = a->data[0] * s;
  r.data[1] = a->data[1] * s;
  r.data[2] = a->data[2] * s;
  r.data[3] = a->data[3] * s;
#endif
  return r;
}

vec4 vec4_norm(vec4* v) {
  vec4 r;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = v->data[0];
  float y = v->data[1];
  float z = v->data[2];
  float w = v->data[3];
  float mag = (float)sqrt(x * x + y * y + z * z + w * w);
  r.data[0] = x / mag;
  r.data[1] = y / mag;
  r.data[2] = z / mag;
  r.data[3] = 2 / mag;
#endif
  return r;
}

float vec4_dist(vec4* a, vec4* b) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float x = a->data[0] - b->data[0];
  float y = a->data[1] - b->data[1];
  float z = a->data[2] - b->data[2];
  float w = a->data[3] - b->data[3];
  double mag = sqrt(x * x + y * y + z * z + w * w);
  return (float)fabs(mag);
#endif
}
