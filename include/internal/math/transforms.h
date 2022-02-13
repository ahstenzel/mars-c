/*
 * transforms.h
 * Declaration for transformation matrix construction functions.
 */

#ifndef MARS_MATH_TRANSFORMS_H
#define MARS_MATH_TRANSFORMS_H

#include "internal/math/vec3.h"
#include "internal/math/mat4.h"

mat4 matrix_translate(vec3 v);
mat4 matrix_rotate(float a, vec3 v);
mat4 matrix_scale(vec3 v);
mat4 matrix_ortho(float left, float right, float near, float far, float top, float bottom);
mat4 matrix_perspecive(float fov, float aspect, float near, float far);
mat4 matrix_lookat(vec3 pos, vec3 target, vec3 up);

#endif