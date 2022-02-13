/*
 * mat3.c
 * Declaration for 3x3 matrix of floats in column-major order.
 */

#include "internal/math/mat3.h"

/* mat3 functions */
mat3 mat3_make(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3) {
  mat3 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = x1;
  n.data[1] = y1;
  n.data[2] = z1;
  n.data[3] = x2;
  n.data[4] = y2;
  n.data[5] = z2;
  n.data[6] = x3;
  n.data[7] = y3;
  n.data[8] = z3;
#endif
  return n;
}

mat3 mat3_dup(float data[]) {
  mat3 m;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  memcpy(m.data, data, 9 * sizeof(float));
#endif
  return m;
}

mat3 mat3_makeid() {
  mat3 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = 1.0f;
  n.data[1] = 0.0f;
  n.data[2] = 0.0f;
  n.data[3] = 0.0f;
  n.data[4] = 1.0f;
  n.data[5] = 0.0f;
  n.data[6] = 0.0f;
  n.data[7] = 0.0f;
  n.data[8] = 1.0f;
#endif
  return n;
}

bool mat3_equal(mat3 a, mat3 b) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (a.data[0] == b.data[0] &&
          a.data[1] == b.data[1] &&
          a.data[2] == b.data[2] &&
          a.data[3] == b.data[3] &&
          a.data[4] == b.data[4] &&
          a.data[5] == b.data[5] &&
          a.data[6] == b.data[6] &&
          a.data[7] == b.data[7] &&
          a.data[8] == b.data[8]);
#endif
}

bool mat3_equalid(mat3 m) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  return (m.data[0] == 1.0f &&
          m.data[1] == 0.0f &&
          m.data[2] == 0.0f &&
          m.data[3] == 0.0f &&
          m.data[4] == 1.0f &&
          m.data[5] == 0.0f &&
          m.data[6] == 0.0f &&
          m.data[7] == 0.0f &&
          m.data[8] == 1.0f);
#endif
}

mat3 mat3_add(mat3 a, mat3 b) {
  mat3 n;
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
  n.data[4] = a.data[4] + b.data[4];
  n.data[5] = a.data[5] + b.data[5];
  n.data[6] = a.data[6] + b.data[6];
  n.data[7] = a.data[7] + b.data[7];
  n.data[8] = a.data[8] + b.data[8];
#endif
  return n;
}

mat3 mat3_sub(mat3 a, mat3 b) {
  mat3 n;
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
  n.data[4] = a.data[4] - b.data[4];
  n.data[5] = a.data[5] - b.data[5];
  n.data[6] = a.data[6] - b.data[6];
  n.data[7] = a.data[7] - b.data[7];
  n.data[8] = a.data[8] - b.data[8];
#endif
  return n;
}

mat3 mat3_scale(mat3 m, float s) {
  mat3 n;
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
  n.data[4] = m.data[4] * s;
  n.data[5] = m.data[5] * s;
  n.data[6] = m.data[6] * s;
  n.data[7] = m.data[7] * s;
  n.data[8] = m.data[8] * s;
#endif
  return n;
}

mat3 mat3_mul(mat3 a, mat3 b) {
  mat3 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = (a.data[0] * b.data[0]) + (a.data[3] * b.data[1]) + (a.data[6] * b.data[2]);
  n.data[1] = (a.data[1] * b.data[0]) + (a.data[4] * b.data[1]) + (a.data[7] * b.data[2]);
  n.data[2] = (a.data[2] * b.data[0]) + (a.data[5] * b.data[1]) + (a.data[8] * b.data[2]);

  n.data[3] = (a.data[0] * b.data[3]) + (a.data[3] * b.data[4]) + (a.data[6] * b.data[5]);
  n.data[4] = (a.data[1] * b.data[3]) + (a.data[4] * b.data[4]) + (a.data[7] * b.data[5]);
  n.data[5] = (a.data[2] * b.data[3]) + (a.data[5] * b.data[4]) + (a.data[8] * b.data[5]);

  n.data[6] = (a.data[0] * b.data[6]) + (a.data[3] * b.data[7]) + (a.data[6] * b.data[8]);
  n.data[7] = (a.data[1] * b.data[6]) + (a.data[4] * b.data[7]) + (a.data[7] * b.data[8]);
  n.data[8] = (a.data[2] * b.data[6]) + (a.data[5] * b.data[7]) + (a.data[8] * b.data[8]);
#endif
  return n;
}

vec3 mat3_vec3_mul(mat3 m, vec3 v) {
  vec3 r;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = (m.data[0] * v.data[0]) + (m.data[3] * v.data[1]) + (m.data[6] * v.data[2]);
  r.data[1] = (m.data[1] * v.data[0]) + (m.data[4] * v.data[1]) + (m.data[7] * v.data[2]);
  r.data[2] = (m.data[2] * v.data[0]) + (m.data[5] * v.data[1]) + (m.data[8] * v.data[2]);
#endif
  return r;
}

mat3 mat3_transpose(mat3 m) {
  mat3 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = m.data[0];
  n.data[1] = m.data[3];
  n.data[2] = m.data[6];
  n.data[3] = m.data[1];
  n.data[4] = m.data[4];
  n.data[5] = m.data[7];
  n.data[6] = m.data[2];
  n.data[7] = m.data[5];
  n.data[8] = m.data[8];
#endif
  return n;
}

float mat3_det(mat3 m) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float det = 0.0f;

  mat2 n = mat3_submatrix(m, 0, 0);
  det += m.data[0] * mat2_det(n);

  n = mat3_submatrix(m, 0, 1);
  det -= m.data[3] * mat2_det(n);

  n = mat3_submatrix(m, 0, 2);
  det += m.data[6] * mat2_det(n);

  return det;
#endif
}

mat2 mat3_submatrix(mat3 m, unsigned int row, unsigned int col) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  unsigned int ind_0 = 0;
  if (row == (ind_0 % 3)) { ind_0 += 1; }
  if (col == (ind_0 / 3)) { ind_0 += 3; }

  unsigned int ind_1 = ind_0 + 1;
  if (row == (ind_1 % 3)) { ind_1 += 1; }
  if (col == (ind_1 / 3)) { ind_1 += 3; }
  
  unsigned int ind_2 = ind_0 + 3;
  if (row == (ind_2 % 3)) { ind_2 += 1; }
  if (col == (ind_2 / 3)) { ind_2 += 3; }
  
  unsigned int ind_3 = ind_2 + 1;
  if (row == (ind_3 % 3)) { ind_3 += 1; }
  if (col == (ind_3 / 3)) { ind_3 += 3; }
  
  mat2 n = mat2_make(m.data[ind_0], m.data[ind_1], m.data[ind_2], m.data[ind_3]);
  return n;
#endif
}