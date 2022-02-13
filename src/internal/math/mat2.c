/*
 * mat2.c
 * Declaration for 2x2 matrix of floats in column-major order.
 */

#include "internal/math/mat2.h"

 /* mat2 functions */
mat2 mat2_make(float x1, float y1, float x2, float y2) {
  mat2 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = x1;
  n.data[1] = y1;
  n.data[2] = x2;
  n.data[3] = y2;
#endif
  return n;
}

mat2 mat2_dup(float data[]) {
  mat2 m;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  memcpy(m.data, data, 4 * sizeof(float));
#endif
  return m;
}

mat2 mat2_makeid() {
  mat2 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = 1.0f;
  n.data[1] = 0.0f;
  n.data[2] = 0.0f;
  n.data[3] = 1.0f;
#endif
  return n;
}

bool mat2_equal(mat2 a, mat2 b) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (a.data[0] == b.data[0] &&
          a.data[1] == b.data[1] &&
          a.data[2] == b.data[2] &&
          a.data[3] == b.data[3]);
#endif
}

bool mat2_equalid(mat2 m) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (m.data[0] == 1.0f &&
          m.data[1] == 0.0f &&
          m.data[2] == 0.0f &&
          m.data[3] == 1.0f);
#endif
}

mat2 mat2_add(mat2 a, mat2 b) {
  mat2 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = a.data[0] + b.data[0];
  n.data[1] = a.data[1] + b.data[1];
  n.data[2] = a.data[2] + b.data[2];
  n.data[3] = a.data[3] + b.data[3];
#endif
  return n;
}

mat2 mat2_sub(mat2 a, mat2 b) {
  mat2 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = a.data[0] - b.data[0];
  n.data[1] = a.data[1] - b.data[1];
  n.data[2] = a.data[2] - b.data[2];
  n.data[3] = a.data[3] - b.data[3];
#endif
  return n;
}

mat2 mat2_scale(mat2 m, float s) {
  mat2 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = m.data[0] * s;
  n.data[1] = m.data[1] * s;
  n.data[2] = m.data[2] * s;
  n.data[3] = m.data[3] * s;
#endif
  return n;
}

mat2 mat2_mul(mat2 a, mat2 b) {
  mat2 n;
#if defined(CAM_SIMD_AVX)
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = (a.data[0] * b.data[0]) + (a.data[2] * b.data[1]);
  n.data[1] = (a.data[1] * b.data[0]) + (a.data[3] * b.data[1]);
  n.data[2] = (a.data[0] * b.data[2]) + (a.data[2] * b.data[3]);
  n.data[3] = (a.data[1] * b.data[2]) + (a.data[3] * b.data[3]);
#endif
  return n;
}

vec2 mat2_vec2_mul(mat2 m, vec2 v) {
  vec2 r;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = (m.data[0] * v.data[0]) + (m.data[2] * v.data[1]);
  r.data[1] = (m.data[1] * v.data[0]) + (m.data[3] * v.data[1]);
#endif
  return r;
}

mat2 mat2_transpose(mat2 m) {
  mat2 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = m.data[0];
  n.data[1] = m.data[2];
  n.data[2] = m.data[1];
  n.data[3] = m.data[3];
#endif
  return n;
}

float mat2_det(mat2 m) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return ((m.data[0] * m.data[3]) - (m.data[1] * m.data[2]));
#endif
}