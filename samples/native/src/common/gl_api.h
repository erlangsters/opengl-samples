//
// Copyright (c) 2025, Byteplug LLC.
//
// This source file is part of a project made by the Erlangsters community and
// is released under the MIT license. Please refer to the LICENSE.md file that
// can be found at the root of the project repository.
//
// Written by Jonathan De Wachter <jonathan.dewachter@byteplug.io>
//
#ifndef GL_API_H
#define GL_API_H

#define SAMPLE_API_GL 1
#define SAMPLE_API_GLES 2

#if defined(OPENGL_VERSION_33)
    #define SAMPLE_OPENGL_API SAMPLE_API_GL
    #define SAMPLE_OPENGL_VERSION_MAJOR 3
    #define SAMPLE_OPENGL_VERSION_MINOR 3
#elif defined(OPENGL_VERSION_41)
    #define SAMPLE_OPENGL_API SAMPLE_API_GL
    #define SAMPLE_OPENGL_VERSION_MAJOR 4
    #define SAMPLE_OPENGL_VERSION_MINOR 1
#elif defined(OPENGL_VERSION_46)
    #define SAMPLE_OPENGL_API SAMPLE_API_GL
    #define SAMPLE_OPENGL_VERSION_MAJOR 4
    #define SAMPLE_OPENGL_VERSION_MINOR 6
#elif defined(OPENGL_ES_VERSION_20)
    #define SAMPLE_OPENGL_API SAMPLE_API_GLES
    #define SAMPLE_OPENGL_VERSION_MAJOR 2
    #define SAMPLE_OPENGL_VERSION_MINOR 0
#elif defined(OPENGL_ES_VERSION_30)
    #define SAMPLE_OPENGL_API SAMPLE_API_GLES
    #define SAMPLE_OPENGL_VERSION_MAJOR 3
    #define SAMPLE_OPENGL_VERSION_MINOR 0
#elif defined(OPENGL_ES_VERSION_31)
    #define SAMPLE_OPENGL_API SAMPLE_API_GLES
    #define SAMPLE_OPENGL_VERSION_MAJOR 3
    #define SAMPLE_OPENGL_VERSION_MINOR 1
#elif defined(OPENGL_ES_VERSION_32)
    #define SAMPLE_OPENGL_API SAMPLE_API_GLES
    #define SAMPLE_OPENGL_VERSION_MAJOR 3
    #define SAMPLE_OPENGL_VERSION_MINOR 2
#else
    #error "Unsupported OpenGL version."
#endif

#if SAMPLE_OPENGL_API == SAMPLE_API_GL
    // Linux desktop GL headers do not consistently expose modern core
    // prototypes unless GL_GLEXT_PROTOTYPES is set before including glext.h.
    #ifndef GL_GLEXT_PROTOTYPES
        #define GL_GLEXT_PROTOTYPES 1
    #endif
    #include <GL/gl.h>
    #include <GL/glext.h>
#elif SAMPLE_OPENGL_VERSION_MAJOR == 2
    #include <GLES2/gl2.h>
#else
    #include <GLES3/gl3.h>
#endif

#endif // GL_API_H