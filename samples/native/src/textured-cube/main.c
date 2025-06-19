//
// Copyright (c) 2025, Byteplug LLC.
//
// This source file is part of a project made by the Erlangsters community and
// is released under the MIT license. Please refer to the LICENSE.md file that
// can be found at the root of the project repository.
//
// Written by Jonathan De Wachter <jonathan.dewachter@byteplug.io>
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#if defined(OPENGL_VERSION_33) || defined(OPENGL_VERSION_46)
    #include <GL/gl.h>
#elif defined(OPENGL_ES_VERSION_20)
    #include <GLES2/gl2.h>
#elif defined(OPENGL_ES_VERSION_32)
    #include <GLES3/gl3.h>
#else
    #error "Unsupported OpenGL version."
#endif
#include <EGL/egl.h>
#include <GLFW/glfw3.h>
#include "matrix.h"
#include "window.h"

const char* vertexShaderSource =
    "precision mediump float;\n"
    "attribute vec3 vertPosition;\n"
    "attribute vec2 vertTexCoord;\n"
    "varying vec2 fragTexCoord;\n"
    "uniform mat4 mWorld;\n"
    "uniform mat4 mView;\n"
    "uniform mat4 mProj;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    fragTexCoord = vertTexCoord;\n"
    "    gl_Position = mProj * mView * mWorld * vec4(vertPosition, 1.0);\n"
    "}\n";

const char* fragmentShaderSource =
    "precision mediump float;\n"
    "varying vec2 fragTexCoord;\n"
    "uniform sampler2D texture;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = texture2D(texture, fragTexCoord);\n"
    "}\n";

void generateCheckerTexture(unsigned char* data, int width, int height,
                          unsigned char r1, unsigned char g1, unsigned char b1,
                          unsigned char r2, unsigned char g2, unsigned char b2) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 4;
            int isAlternate = (x / 2 + y / 2) % 2;

            data[index + 0] = isAlternate ? r1 : r2;
            data[index + 1] = isAlternate ? g1 : g2;
            data[index + 2] = isAlternate ? b1 : b2;
            data[index + 3] = 255;
        }
    }
}

int main() {
    GLFWwindow* window;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    if (initializeWindow(&window, &display, &context, &surface, 640, 480, "Erlangsters - Textured Cube") != 0) {
        return -1;
    }

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Vertex shader compilation failed: %s\n", infoLog);
        return -1;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Fragment shader compilation failed: %s\n", infoLog);
        return -1;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Shader program linking failed: %s\n", infoLog);
        return -1;
    }

    float vertices[] = {
        // Format: X, Y, Z, U, V
        // Top
        -1.0f,  1.0f, -1.0f,   0.0f, 0.0f,
        -1.0f,  1.0f,  1.0f,   0.0f, 1.0f,
         1.0f,  1.0f,  1.0f,   1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,   1.0f, 0.0f,

        // Left
        -1.0f,  1.0f,  1.0f,   0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,   1.0f, 0.0f,

        // Right
         1.0f,  1.0f,  1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,   0.0f, 1.0f,
         1.0f, -1.0f, -1.0f,   1.0f, 1.0f,
         1.0f,  1.0f, -1.0f,   1.0f, 0.0f,

        // Front
         1.0f,  1.0f,  1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,  1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f,  1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f,  1.0f,   1.0f, 0.0f,

        // Back
         1.0f,  1.0f, -1.0f,   0.0f, 0.0f,
         1.0f, -1.0f, -1.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,   1.0f, 1.0f,
        -1.0f,  1.0f, -1.0f,   1.0f, 0.0f,

        // Bottom
        -1.0f, -1.0f, -1.0f,   0.0f, 0.0f,
        -1.0f, -1.0f,  1.0f,   0.0f, 1.0f,
         1.0f, -1.0f,  1.0f,   1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,   1.0f, 0.0f
    };

    unsigned short indices[] = {
        0, 1, 2,    0, 2, 3,    // Top
        5, 4, 6,    6, 4, 7,    // Left
        8, 9, 10,   8, 10, 11,  // Right
        13, 12, 14, 15, 14, 12, // Front
        16, 17, 18, 16, 18, 19, // Back
        21, 20, 22, 22, 20, 23  // Bottom
    };

    GLuint VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    const int texWidth = 16;
    const int texHeight = 16;
    unsigned char textureData[texWidth * texHeight * 4];
    generateCheckerTexture(textureData, texWidth, texHeight, 255, 0, 0, 128, 0, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    GLint posAttrib = glGetAttribLocation(shaderProgram, "vertPosition");
    GLint texCoordAttrib = glGetAttribLocation(shaderProgram, "vertTexCoord");

    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(posAttrib);
    glEnableVertexAttribArray(texCoordAttrib);

    GLint worldUniform = glGetUniformLocation(shaderProgram, "mWorld");
    GLint viewUniform = glGetUniformLocation(shaderProgram, "mView");
    GLint projUniform = glGetUniformLocation(shaderProgram, "mProj");

    mat4 world, view, proj;
    mat4 rotatedY, rotatedX;

    mat4_identity(world);
    mat4_look_at(view,
        0, 0, -8,
        0, 0, 0,
        0, 1, 0
    );

    mat4_perspective(proj,
        45.0f * M_PI / 180.0f,
        640.0f / 480.0f,
        0.1f,
        1000.0f
    );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(worldUniform, 1, GL_FALSE, (float*)world);
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, (float*)view);
    glUniformMatrix4fv(projUniform, 1, GL_FALSE, (float*)proj);

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        double currentTime = glfwGetTime();
        float angle = (float)currentTime;

        mat4_identity(world);
        mat4_rotate_y(rotatedY, world, angle);
        mat4_rotate_x(world, rotatedY, angle * 0.25f);

        glClearColor(0.75f, 0.85f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(worldUniform, 1, GL_FALSE, (float*)world);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);

        eglSwapBuffers(display, surface);
        glfwPollEvents();
    }

    glDeleteTextures(1, &texture);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    terminateWindow(window);

    return 0;
}
