//
// Copyright (c) 2025, Byteplug LLC.
//
// This source file is part of a project made by the Erlangsters community and
// is released under the MIT license. Please refer to the LICENSE.md file that
// can be found at the root of the project repository.
//
// Written by Jonathan De Wachter <jonathan.dewachter@byteplug.io>
//
#if defined(_WIN32)
    #define GLFW_EXPOSE_NATIVE_WIN32
#elif defined(__APPLE__)
    #define GLFW_EXPOSE_NATIVE_COCOA
#elif defined(__linux__)
    #define GLFW_EXPOSE_NATIVE_X11
#else
    #error "Unsupported platform."
#endif

#include <string.h>
#include "gl_api.h"
#include "window.h"
#include <GLFW/glfw3native.h>
#include <stdio.h>

#ifndef EGL_OPENGL_ES3_BIT
    #define EGL_OPENGL_ES3_BIT EGL_OPENGL_ES3_BIT_KHR
#endif

static EGLNativeWindowType get_native_window_handle(GLFWwindow* window)
{
#if defined(_WIN32)
    return (EGLNativeWindowType)glfwGetWin32Window(window);
#elif defined(__APPLE__)
    return (EGLNativeWindowType)glfwGetCocoaWindow(window);
#elif defined(__linux__)
    return (EGLNativeWindowType)glfwGetX11Window(window);
#endif
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    (void)window;
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
    // Initialize EGL.
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

    // Set the rendering API.
    #if SAMPLE_OPENGL_API == SAMPLE_API_GL
        eglBindAPI(EGL_OPENGL_API);
    #elif SAMPLE_OPENGL_API == SAMPLE_API_GLES
        eglBindAPI(EGL_OPENGL_ES_API);
    #else
        #error "Unsupported OpenGL version."
    #endif

    // Choose an EGL config.
    #if SAMPLE_OPENGL_API == SAMPLE_API_GL
        EGLint renderableType = EGL_OPENGL_BIT;
    #elif SAMPLE_OPENGL_VERSION_MAJOR == 2
        EGLint renderableType = EGL_OPENGL_ES2_BIT;
    #else
        EGLint renderableType = EGL_OPENGL_ES2_BIT | EGL_OPENGL_ES3_BIT;
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
        return -1;
    }

    // Create an EGL context.
    EGLint contextAttribs[] = {
        EGL_CONTEXT_MAJOR_VERSION, SAMPLE_OPENGL_VERSION_MAJOR,
        EGL_CONTEXT_MINOR_VERSION, SAMPLE_OPENGL_VERSION_MINOR,
        EGL_NONE
    };
    *context = eglCreateContext(*display, config, EGL_NO_CONTEXT, contextAttribs);
    if (*context == EGL_NO_CONTEXT) {
        fprintf(stderr, "Failed to create EGL context\n");
        eglTerminate(*display);
        return -1;
    }

    // Initialize GLFW.
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    // Create a GLFW window (without OpenGL context).
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    *window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!*window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        eglDestroyContext(*display, *context);
        eglTerminate(*display);
        return -1;
    }

    // Requesting an explicit GLES 3.x context through EGL depends on
    // EGL_KHR_create_context on the Linux drivers we target today.
    if (SAMPLE_OPENGL_API == SAMPLE_API_GLES && SAMPLE_OPENGL_VERSION_MAJOR >= 3) {
        const char* egl_extensions = eglQueryString(*display, EGL_EXTENSIONS);
        if (!egl_extensions || strstr(egl_extensions, "EGL_KHR_create_context") == NULL) {
            fprintf(stderr, "EGL_KHR_create_context is required for OpenGL ES 3.x contexts\n");
            glfwDestroyWindow(*window);
            glfwTerminate();
            eglDestroyContext(*display, *context);
            eglTerminate(*display);
            return -1;
        }
    }

    // Set the GLFW window size callback to adjust the OpenGL viewport.
    glfwSetWindowSizeCallback(*window, window_size_callback);

    // GLFW gives us the platform-native window object, while EGL turns it into
    // the presentation surface used by the samples.
    *surface = eglCreateWindowSurface(*display, config, get_native_window_handle(*window), NULL);
    if (*surface == EGL_NO_SURFACE) {
        fprintf(stderr, "Failed to create EGL surface\n");
        glfwDestroyWindow(*window);
        glfwTerminate();
        eglDestroyContext(*display, *context);
        eglTerminate(*display);
        return -1;
    }

    // Make the EGL context current.
    if (!eglMakeCurrent(*display, *surface, *surface, *context)) {
        fprintf(stderr, "Failed to make EGL context current\n");
        eglDestroySurface(*display, *surface);
        glfwDestroyWindow(*window);
        glfwTerminate();
        eglDestroyContext(*display, *context);
        eglTerminate(*display);
        return -1;
    }

    // Display the OpenGL version and EGL version.
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
