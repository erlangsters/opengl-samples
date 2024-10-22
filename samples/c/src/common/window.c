#include "window.h"
#include <stdio.h>

int initializeWindow(GLFWwindow** window, EGLDisplay* display, int width, int height, const char* title) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

    *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!*window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(*window);

    *display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(*display, NULL, NULL);

    const char* egl_version = eglQueryString(*display, EGL_VERSION);
    printf("EGL Version: %s\n", egl_version);

    const char* gl_version = (const char*)glGetString(GL_VERSION);
    printf("OpenGL ES Version: %s\n", gl_version);

    return 0;
}

void terminateWindow(GLFWwindow* window) {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}
