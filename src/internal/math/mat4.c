/*
 * mat4.c
 * Declaration for 4x4 matrix of floats in column-major order.
 */

#include "internal/math/mat3.h"
#include "internal/math/mat4.h"

/* mat4 functions */
mat4 mat4_make(float x1, float y1, float z1, float w1, float x2, float y2, float z2, float w2, float x3, float y3, float z3, float w3, float x4, float y4, float z4, float w4) {
  mat4 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = x1;
  n.data[1] = y1;
  n.data[2] = z1;
  n.data[3] = w1;
  n.data[4] = x2;
  n.data[5] = y2;
  n.data[6] = z2;
  n.data[7] = w2;
  n.data[8] = x3;
  n.data[9] = y3;
  n.data[10] = z3;
  n.data[11] = w3;
  n.data[12] = x4;
  n.data[13] = y4;
  n.data[14] = z4;
  n.data[15] = w4;
#endif
  return n;
}

mat4 mat4_dup(float data[]) {
  mat4 m;
#if defined(MARS_SIMD_AVX)
  // Intel AVX
#elif defined(MARS_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  memcpy(m.data, data, 16 * sizeof(float));
#endif
  return m;
}

mat4 mat4_makeid() {
  mat4 n;
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
  n.data[4] = 0.0f;
  n.data[5] = 1.0f;
  n.data[6] = 0.0f;
  n.data[7] = 0.0f;
  n.data[8] = 0.0f;
  n.data[9] = 0.0f;
  n.data[10] = 1.0f;
  n.data[11] = 0.0f;
  n.data[12] = 0.0f;
  n.data[13] = 0.0f;
  n.data[14] = 0.0f;
  n.data[15] = 1.0f;
#endif
  return n;
}

bool mat4_equal(mat4 a, mat4 b) {
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
          a.data[8] == b.data[8] &&
          a.data[9] == b.data[9] &&
          a.data[10] == b.data[10] &&
          a.data[11] == b.data[11] &&
          a.data[12] == b.data[12] &&
          a.data[13] == b.data[13] &&
          a.data[14] == b.data[14] &&
          a.data[15] == b.data[15]);
#endif
}

bool mat4_equalid(mat4 m) {
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
          m.data[4] == 0.0f &&
          m.data[5] == 1.0f &&
          m.data[6] == 0.0f &&
          m.data[7] == 0.0f &&
          m.data[8] == 0.0f &&
          m.data[9] == 0.0f &&
          m.data[10] == 1.0f &&
          m.data[11] == 0.0f &&
          m.data[12] == 0.0f &&
          m.data[13] == 0.0f &&
          m.data[14] == 0.0f &&
          m.data[15] == 1.0f);
#endif
}

mat4 mat4_add(mat4 a, mat4 b) {
  mat4 n;
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
  n.data[9] = a.data[9] + b.data[9];
  n.data[10] = a.data[10] + b.data[10];
  n.data[11] = a.data[11] + b.data[11];
  n.data[12] = a.data[12] + b.data[12];
  n.data[13] = a.data[13] + b.data[13];
  n.data[14] = a.data[14] + b.data[14];
  n.data[15] = a.data[15] + b.data[15];
#endif
  return n;
}

mat4 mat4_sub(mat4 a, mat4 b) {
  mat4 n;
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
  n.data[9] = a.data[9] - b.data[9];
  n.data[10] = a.data[10] - b.data[10];
  n.data[11] = a.data[11] - b.data[11];
  n.data[12] = a.data[12] - b.data[12];
  n.data[13] = a.data[13] - b.data[13];
  n.data[14] = a.data[14] - b.data[14];
  n.data[15] = a.data[15] - b.data[15];
#endif
  return n;
}

mat4 mat4_scale(mat4 m, float s) {
  mat4 n;
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
  n.data[9] = m.data[9] * s;
  n.data[10] = m.data[10] * s;
  n.data[11] = m.data[11] * s;
  n.data[12] = m.data[12] * s;
  n.data[13] = m.data[13] * s;
  n.data[14] = m.data[14] * s;
  n.data[15] = m.data[15] * s;
#endif
  return n;
}

mat4 mat4_mul(mat4 a, mat4 b) {
  mat4 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = (a.data[0] * b.data[0]) + (a.data[4] * b.data[1]) + (a.data[8] * b.data[2]) + (a.data[12] * b.data[3]);
  n.data[1] = (a.data[1] * b.data[0]) + (a.data[5] * b.data[1]) + (a.data[9] * b.data[2]) + (a.data[13] * b.data[3]);
  n.data[2] = (a.data[2] * b.data[0]) + (a.data[6] * b.data[1]) + (a.data[10]* b.data[2]) + (a.data[14] * b.data[3]);
  n.data[3] = (a.data[3] * b.data[0]) + (a.data[7] * b.data[1]) + (a.data[11]* b.data[2]) + (a.data[15] * b.data[3]);

  n.data[4] = (a.data[0] * b.data[4]) + (a.data[4] * b.data[5]) + (a.data[8] * b.data[6]) + (a.data[12] * b.data[7]);
  n.data[5] = (a.data[1] * b.data[4]) + (a.data[5] * b.data[5]) + (a.data[9] * b.data[6]) + (a.data[13] * b.data[7]);
  n.data[6] = (a.data[2] * b.data[4]) + (a.data[6] * b.data[5]) + (a.data[10]* b.data[6]) + (a.data[14] * b.data[7]);
  n.data[7] = (a.data[3] * b.data[4]) + (a.data[7] * b.data[5]) + (a.data[11]* b.data[6]) + (a.data[15] * b.data[7]);

  n.data[8] = (a.data[0] * b.data[8]) + (a.data[4] * b.data[9]) + (a.data[8] * b.data[10]) + (a.data[12] * b.data[11]);
  n.data[9] = (a.data[1] * b.data[8]) + (a.data[5] * b.data[9]) + (a.data[9] * b.data[10]) + (a.data[13] * b.data[11]);
  n.data[10]= (a.data[2] * b.data[8]) + (a.data[6] * b.data[9]) + (a.data[10]* b.data[10]) + (a.data[14] * b.data[11]);
  n.data[11]= (a.data[3] * b.data[8]) + (a.data[7] * b.data[9]) + (a.data[11]* b.data[10]) + (a.data[15] * b.data[11]);

  n.data[12]= (a.data[0] * b.data[12]) + (a.data[4] * b.data[13]) + (a.data[8] * b.data[14]) + (a.data[12] * b.data[15]);
  n.data[13]= (a.data[1] * b.data[12]) + (a.data[5] * b.data[13]) + (a.data[9] * b.data[14]) + (a.data[13] * b.data[15]);
  n.data[14]= (a.data[2] * b.data[12]) + (a.data[6] * b.data[13]) + (a.data[10]* b.data[14]) + (a.data[14] * b.data[15]);
  n.data[15]= (a.data[3] * b.data[12]) + (a.data[7] * b.data[13]) + (a.data[11]* b.data[14]) + (a.data[15] * b.data[15]);
#endif
  return n;
}

vec4 mat4_vec4_mul(mat4 m, vec4 v) {
  vec4 r;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  r.data[0] = (m.data[0] * v.data[0]) + (m.data[4] * v.data[1]) + (m.data[8] * v.data[2]) + (m.data[12] * v.data[3]);
  r.data[1] = (m.data[1] * v.data[0]) + (m.data[5] * v.data[1]) + (m.data[9] * v.data[2]) + (m.data[13] * v.data[3]);
  r.data[2] = (m.data[2] * v.data[0]) + (m.data[6] * v.data[1]) + (m.data[10]* v.data[2]) + (m.data[14] * v.data[3]);
  r.data[3] = (m.data[3] * v.data[0]) + (m.data[7] * v.data[1]) + (m.data[11]* v.data[2]) + (m.data[15] * v.data[3]);
#endif
  return r;
}

mat4 mat4_transpose(mat4 m) {
  mat4 n;
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  n.data[0] = m.data[0];
  n.data[1] = m.data[4];
  n.data[2] = m.data[8];
  n.data[3] = m.data[12];
  n.data[4] = m.data[1];
  n.data[5] = m.data[5];
  n.data[6] = m.data[9];
  n.data[7] = m.data[13];
  n.data[8] = m.data[2];
  n.data[9] = m.data[6];
  n.data[10] = m.data[10];
  n.data[11] = m.data[14];
  n.data[12] = m.data[3];
  n.data[13] = m.data[7];
  n.data[14] = m.data[11];
  n.data[15] = m.data[15];
#endif
  return n;
}

float mat4_det(mat4 m) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  float det = 0.0f;

  mat3 n = mat4_submatrix(m, 0, 0);
  det += m.data[0] * mat3_det(n);

  n = mat4_submatrix(m, 0, 1);
  det += m.data[4] * mat3_det(n);

  n = mat4_submatrix(m, 0, 2);
  det += m.data[8] * mat3_det(n);

  n = mat4_submatrix(m, 0, 3);
  det += m.data[12] * mat3_det(n);

  return det;
#endif
}

mat3 mat4_submatrix(mat4 m, unsigned int row, unsigned int col) {
#if defined(CAM_SIMD_AVX)
  // Intel AVX
#elif defined(CAM_SIMD_NEON)
  // AMD NEON
#else
  // No SIMD intrinsics
  unsigned int ind_0 = 0;
  if (row == ind_0) { ind_0 += 1; }
  if (col == ind_0) { ind_0 += 4; }

  unsigned int ind_1 = ind_0 + 1;
  if (row == ind_1) { ind_1 += 1; }
  if (col == ind_1) { ind_1 += 4; }

  unsigned int ind_2 = ind_0 + 2;
  if (row == ind_2) { ind_2 += 1; }
  if (col == ind_2) { ind_2 += 4; }

  unsigned int ind_3 = ind_0 + 4;
  if (row == ind_3) { ind_3 += 1; }
  if (col == ind_3) { ind_3 += 4; }

  unsigned int ind_4 = ind_3 + 1;
  if (row == ind_4) { ind_4 += 1; }
  if (col == ind_4) { ind_4 += 4; }

  unsigned int ind_5 = ind_4 + 2;
  if (row == ind_5) { ind_5 += 1; }
  if (col == ind_5) { ind_5 += 4; }

  unsigned int ind_6 = ind_3 + 4;
  if (row == ind_6) { ind_6 += 1; }
  if (col == ind_6) { ind_6 += 4; }

  unsigned int ind_7 = ind_6 + 1;
  if (row == ind_7) { ind_7 += 1; }
  if (col == ind_7) { ind_7 += 4; }

  unsigned int ind_8 = ind_7 + 2;
  if (row == ind_8) { ind_8 += 1; }
  if (col == ind_8) { ind_8 += 4; }

  mat3 n = mat3_make(m.data[ind_0], m.data[ind_1], m.data[ind_2], 
                     m.data[ind_3], m.data[ind_4], m.data[ind_5],
                     m.data[ind_6], m.data[ind_7], m.data[ind_8]);
  return n;
#endif
}