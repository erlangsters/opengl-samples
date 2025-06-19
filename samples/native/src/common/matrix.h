//
// Copyright (c) 2025, Byteplug LLC.
//
// This source file is part of a project made by the Erlangsters community and
// is released under the MIT license. Please refer to the LICENSE.md file that
// can be found at the root of the project repository.
//
// Written by Jonathan De Wachter <jonathan.dewachter@byteplug.io>
//
#ifndef MATRIX_H
#define MATRIX_H

typedef float mat4[16];

void mat4_identity(mat4 m);
void mat4_perspective(mat4 m, float fovy, float aspect, float near, float far);

void mat4_look_at(mat4 m, float eyeX, float eyeY, float eyeZ,
    float centerX, float centerY, float centerZ,
    float upX, float upY, float upZ);

void mat4_rotate_y(mat4 result, const mat4 m, float angle);
void mat4_rotate_x(mat4 result, const mat4 m, float angle);

#endif // MATRIX_H
