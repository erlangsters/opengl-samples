//
// Copyright (c) 2025, Byteplug LLC.
//
// This source file is part of a project made by the Erlangsters community and
// is released under the MIT license. Please refer to the LICENSE.md file that
// can be found at the root of the project repository.
//
// Written by Jonathan De Wachter <jonathan.dewachter@byteplug.io>
//
#ifndef WINDOW_H
#define WINDOW_H

#include <GLFW/glfw3.h>
#include <EGL/egl.h>

int initializeWindow(
    GLFWwindow** window,
    EGLDisplay* display,
    int width,
    int height,
    const char* title
);

void terminateWindow(GLFWwindow* window);

#endif // WINDOW_H
