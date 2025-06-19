//
// Copyright (c) 2025, Byteplug LLC.
//
// This source file is part of a project made by the Erlangsters community and
// is released under the MIT license. Please refer to the LICENSE.md file that
// can be found at the root of the project repository.
//
// Written by Jonathan De Wachter <jonathan.dewachter@byteplug.io>
//
#include "window.h"
#include <stdio.h>

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int initializeWindow(
    GLFWwindow** window,
    EGLDisplay* display,
    EGLContext* context,
    EGLSurface* surface,
    int width, int height,
    const char* title
) {
    *display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (*display == EGL_NO_DISPLAY) {
        fprintf(stderr, "Failed to get EGL display\n");
        glfwTerminate();
            return -1;
        }

    if (!eglInitialize(*display, NULL, NULL)) {
        fprintf(stderr, "Failed to initialize EGL\n");
        glfwTerminate();
        return -1;
    }

    EGLint renderableType;
    EGLint versionMajor;
    EGLint versionMinor;
#if defined(OPENGL_VERSION_33)
    renderableType = EGL_OPENGL_BIT;
    versionMajor = 3;
    versionMinor = 3;
    eglBindAPI(EGL_OPENGL_API);
#elif defined(OPENGL_VERSION_46)
    renderableType = EGL_OPENGL_BIT;
    versionMajor = 4;
    versionMinor = 6;
    eglBindAPI(EGL_OPENGL_API);
#elif defined(OPENGL_ES_VERSION_20)
    renderableType = EGL_OPENGL_ES2_BIT;
    versionMajor = 2;
    versionMinor = 0;
    eglBindAPI(EGL_OPENGL_ES_API);
#elif defined(OPENGL_ES_VERSION_32)
    renderableType = EGL_OPENGL_ES2_BIT;
    versionMajor = 3;
    versionMinor = 2;
    eglBindAPI(EGL_OPENGL_ES_API);
#else
    #error "Unsupported OpenGL version."
#endif

    EGLint configAttribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, renderableType,
        EGL_NONE
    };
    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(*display, configAttribs, &config, 1, &numConfigs) || numConfigs < 1) {
        fprintf(stderr, "Failed to choose EGL config\n");
        eglTerminate(*display);
        glfwTerminate();
        return -1;
    }

    EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, versionMajor,
        EGL_CONTEXT_MINOR_VERSION, versionMinor,
        EGL_NONE
    };
    *context = eglCreateContext(*display, config, EGL_NO_CONTEXT, contextAttribs);
    if (*context == EGL_NO_CONTEXT) {
        fprintf(stderr, "Failed to create EGL context\n");
        eglTerminate(*display);
        glfwTerminate();
        return -1;
    }

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!*window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(*window, window_size_callback);

    *surface = eglCreateWindowSurface(*display, config, (EGLNativeWindowType)glfwGetX11Window(*window), NULL);
    if (*surface == EGL_NO_SURFACE) {
        fprintf(stderr, "Failed to create EGL surface\n");
        eglDestroyContext(*display, *context);
        eglTerminate(*display);
        glfwTerminate();
        return -1;
    }

    if (!eglMakeCurrent(*display, *surface, *surface, *context)) {
        fprintf(stderr, "Failed to make EGL context current\n");
        eglDestroySurface(*display, *surface);
        eglDestroyContext(*display, *context);
        eglTerminate(*display);
        glfwTerminate();
        return -1;
    }

    const char* egl_version = eglQueryString(*display, EGL_VERSION);
    printf("EGL Version: %s\n", egl_version);

    const char* gl_version = (const char*)glGetString(GL_VERSION);
    printf("OpenGL Version: %s\n", gl_version);

    return 0;
}

void terminateWindow(GLFWwindow* window) {
    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}
