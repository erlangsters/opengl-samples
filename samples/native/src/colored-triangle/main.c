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
#include "window.h"

#if defined(OPENGL_VERSION_33)
    const char* vertex_shader_src = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 ourColor;
void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
    )";

    const char* fragment_shader_src = R"(
#version 330 core
in vec3 ourColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(ourColor, 1.0);
}
    )";
#elif defined(OPENGL_VERSION_46)
    const char* vertex_shader_src = R"(
#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 ourColor;
void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
    )";

    const char* fragment_shader_src = R"(
#version 460 core
in vec3 ourColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(ourColor, 1.0);
}
    )";
#elif defined(OPENGL_ES_VERSION_20)
    const char* vertex_shader_src = R"(
#version 100
attribute vec3 aPos;
attribute vec3 aColor;
varying vec3 ourColor;
void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
    )";

    const char* fragment_shader_src = R"(
#version 100
precision mediump float;
varying vec3 ourColor;
void main() {
    gl_FragColor = vec4(ourColor, 1.0);
}
    )";
#elif defined(OPENGL_ES_VERSION_32)
    const char* vertex_shader_src = R"(
#version 320 es
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 ourColor;
void main() {
    gl_Position = vec4(aPos, 1.0);
    ourColor = aColor;
}
    )";

    const char* fragment_shader_src = R"(
#version 320 es
precision mediump float;
in vec3 ourColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(ourColor, 1.0);
}
    )";
#else
    #error "Unsupported OpenGL version."
#endif

GLfloat vertices[] = {
     0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f
};

void check_shader_compile_status(GLuint shader) {
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
    }
}

void check_program_link_status(GLuint program) {
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        fprintf(stderr, "ERROR::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
}

int main() {
    GLFWwindow* window;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    if (initializeWindow(&window, &display, &context, &surface, 640, 480, "Erlangsters - Colored Triangle") != 0) {
        return -1;
    }

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);
    check_shader_compile_status(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_src, NULL);
    glCompileShader(fragment_shader);
    check_shader_compile_status(fragment_shader);

    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    check_program_link_status(shader_program);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    GLint posAttrib = 0;
    #if defined(OPENGL_VERSION_ES_20)
        posAttrib = glGetAttribLocation(shader_program, "aPos");
    #endif
    glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(posAttrib);

    // Color attribute
    GLint colorAttrib = 1;
    #if defined(OPENGL_VERSION_ES_20)
        colorAttrib = glGetAttribLocation(shader_program, "aColor");
    #endif
    glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(colorAttrib);

    glBindVertexArray(0); // Unbind VAO

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Render
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // Swap front and back buffers
        eglSwapBuffers(display, surface);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader_program);

    terminateWindow(window);

    return 0;
}
