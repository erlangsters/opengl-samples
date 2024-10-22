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
