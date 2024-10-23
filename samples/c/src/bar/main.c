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
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <GLFW/glfw3.h>
#include "window.h"

int main() {
    GLFWwindow* window;
    EGLDisplay display;
    if (initializeWindow(&window, &display, 640, 480, "OpenGL Samples - Bar") != 0) {
        return -1;
    }

    // To be written.

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // To be written.

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    terminateWindow(window);

    return 0;
}
