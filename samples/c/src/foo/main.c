#include <stdio.h>
#include <stdlib.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <GLFW/glfw3.h>
#include "window.h"

int main() {
    GLFWwindow* window;
    EGLDisplay display;
    if (initializeWindow(&window, &display, 640, 480, "OpenGL Samples - Foo") != 0) {
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
