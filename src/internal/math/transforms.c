/*
 * transforms.c
 * Definitions for transformation matrix construction functions.
 */

#include "internal/math/transforms.h"

mat4 matrix_translate(vec3 v) {
  float data[16] = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    vec3_getx(v), vec3_gety(v), vec3_getz(v), 1.0f
  };
  return mat4_dup(data);
}

mat4 matrix_rotate(float a, vec3 v) {
  float data[16] = { 0 };
  float c = (float)cos(a);
  float s = (float)sin(a);
  float x = vec3_getx(v);
  float y = vec3_gety(v);
  float z = vec3_getz(v);

  data[0] = c + (x * x * (1.0f - c));
  data[1] = (y * x * (1.0f - c)) + (z * s);
  data[2] = (z * x * (1.0f - c)) - (y * s);

  data[4] = (x * y * (1.0f - c)) - (z * s);
  data[5] = c + (y * y * (1.0f - c));
  data[6] = (z * y * (1.0f - c)) + (x * s);

  data[8] = (x * z * (1.0f - c)) + (y * s);
  data[9] = (y * z * (1.0f - c)) - (x * s);
  data[10] = c + (z * z * (1.0f - c));
  data[15] = 1.0f;
  return mat4_dup(data);
}

mat4 matrix_scale(vec3 v) {
  float data[16] = {
    vec3_getx(v), vec3_gety(v), vec3_getz(v), 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
  };
  return mat4_dup(data);
}

mat4 matrix_ortho(float left, float right, float near, float far, float top, float bottom) {
  float data[16] = {
    2.0f / (right - left), 2.0f / (top - bottom), -2.0f / (far - near), 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    (right + left) / (right - left), (top + bottom) / (top - bottom), (far + near) / (far - near), 1.0f
  };
  return mat4_dup(data);
}

mat4 matrix_perspecive(float fov, float aspect, float near, float far) {
  float top = (float)tan(fov / 2.0f) * near;
  float bottom = -top;
  float right = top * aspect;
  float left = bottom * aspect;

  float data[16] = {
    (2.0f * near) / (right - left), (2.0f * near) / (top - bottom), 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    (right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near), -1.0f,
    0.0f, 0.0f, (-2.0f * far * near) / (far - near), 0.0f,
  };
  return mat4_dup(data);
}

mat4 matrix_lookat(vec3 pos, vec3 target, vec3 up) {
  mat4 t1 = mat4_makeid();
  mat4 t2 = matrix_translate(vec3_scale(pos, -1.0f));

  vec3 direc = vec3_norm(vec3_sub(pos, target));
  vec3 right = vec3_norm(vec3_sub(up, direc));
  vec3 camUp = vec3_cross(direc, right);

  mat4_set(t1, vec3_getx(right), 0, 0);
  mat4_set(t1, vec3_getx(camUp), 0, 1);
  mat4_set(t1, vec3_getx(direc), 0, 2);

  mat4_set(t1, vec3_gety(right), 1, 0);
  mat4_set(t1, vec3_gety(camUp), 1, 1);
  mat4_set(t1, vec3_gety(direc), 1, 2);

  mat4_set(t1, vec3_getz(right), 2, 0);
  mat4_set(t1, vec3_getz(camUp), 2, 1);
  mat4_set(t1, vec3_getz(direc), 2, 2);

  return mat4_mul(t1, t2);
}