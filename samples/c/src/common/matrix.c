//
// Copyright (c) 2025, Byteplug LLC.
//
// This source file is part of a project made by the Erlangsters community and
// is released under the MIT license. Please refer to the LICENSE.md file that
// can be found at the root of the project repository.
//
// Written by Jonathan De Wachter <jonathan.dewachter@byteplug.io>
//
#include "matrix.h"
#include <math.h>

// Initialize matrix to identity
void mat4_identity(mat4 m) {
    m[0] = 1.0f; m[4] = 0.0f; m[8] = 0.0f; m[12] = 0.0f;
    m[1] = 0.0f; m[5] = 1.0f; m[9] = 0.0f; m[13] = 0.0f;
    m[2] = 0.0f; m[6] = 0.0f; m[10] = 1.0f; m[14] = 0.0f;
    m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;
}

// Create a perspective projection matrix
void mat4_perspective(mat4 m, float fovy, float aspect, float near, float far) {
    float f = 1.0f / tanf(fovy / 2.0f);

    m[0] = f / aspect; m[4] = 0.0f; m[8] = 0.0f; m[12] = 0.0f;
    m[1] = 0.0f; m[5] = f; m[9] = 0.0f; m[13] = 0.0f;
    m[2] = 0.0f; m[6] = 0.0f; m[10] = (far + near) / (near - far); m[14] = (2.0f * far * near) / (near - far);
    m[3] = 0.0f; m[7] = 0.0f; m[11] = -1.0f; m[15] = 0.0f;
}

// Create a look-at view matrix
void mat4_look_at(mat4 m, float eyeX, float eyeY, float eyeZ,
                         float centerX, float centerY, float centerZ,
                         float upX, float upY, float upZ) {
    // Calculate forward vector
    float fx = centerX - eyeX;
    float fy = centerY - eyeY;
    float fz = centerZ - eyeZ;

    // Normalize forward vector
    float len = sqrtf(fx*fx + fy*fy + fz*fz);
    fx /= len;
    fy /= len;
    fz /= len;

    // Calculate right vector
    float rx = fy * upZ - fz * upY;
    float ry = fz * upX - fx * upZ;
    float rz = fx * upY - fy * upX;

    // Normalize right vector
    len = sqrtf(rx*rx + ry*ry + rz*rz);
    rx /= len;
    ry /= len;
    rz /= len;

    // Calculate up vector
    float ux = ry * fz - rz * fy;
    float uy = rz * fx - rx * fz;
    float uz = rx * fy - ry * fx;

    m[0] = rx; m[4] = ux; m[8] = -fx; m[12] = 0.0f;
    m[1] = ry; m[5] = uy; m[9] = -fy; m[13] = 0.0f;
    m[2] = rz; m[6] = uz; m[10] = -fz; m[14] = 0.0f;
    m[3] = 0.0f; m[7] = 0.0f; m[11] = 0.0f; m[15] = 1.0f;

    // Translate the eye position
    float tx = -(rx * eyeX + ry * eyeY + rz * eyeZ);
    float ty = -(ux * eyeX + uy * eyeY + uz * eyeZ);
    float tz = -(-fx * eyeX - fy * eyeY - fz * eyeZ);

    m[12] = tx;
    m[13] = ty;
    m[14] = tz;
}

// Rotate matrix around Y axis
void mat4_rotate_y(mat4 result, const mat4 m, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    float m0 = m[0], m2 = m[2], m8 = m[8], m10 = m[10];
    float m4 = m[4], m6 = m[6], m12 = m[12], m14 = m[14];

    result[0] = c * m0 - s * m2;
    result[2] = s * m0 + c * m2;
    result[8] = c * m8 - s * m10;
    result[10] = s * m8 + c * m10;

    // Copy the rest of the matrix
    result[1] = m[1]; result[3] = m[3];
    result[4] = m[4]; result[5] = m[5]; result[6] = m[6]; result[7] = m[7];
    result[9] = m[9]; result[11] = m[11];
    result[12] = m[12]; result[13] = m[13]; result[14] = m[14]; result[15] = m[15];
}

// Rotate matrix around X axis
void mat4_rotate_x(mat4 result, const mat4 m, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    float m1 = m[1], m2 = m[2], m5 = m[5], m6 = m[6];
    float m9 = m[9], m10 = m[10], m13 = m[13], m14 = m[14];

    result[1] = c * m1 + s * m2;
    result[2] = -s * m1 + c * m2;
    result[5] = c * m5 + s * m6;
    result[6] = -s * m5 + c * m6;
    result[9] = c * m9 + s * m10;
    result[10] = -s * m9 + c * m10;

    // Copy the rest of the matrix
    result[0] = m[0]; result[3] = m[3]; result[4] = m[4];
    result[7] = m[7]; result[8] = m[8]; result[11] = m[11];
    result[12] = m[12]; result[13] = m[13];
    result[14] = m[14]; result[15] = m[15];
}
