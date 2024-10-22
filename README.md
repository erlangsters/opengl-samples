# OpenGL Samples (C & Erlang)

:construction: It's still in development.

[![Erlangsters Repository](https://img.shields.io/badge/erlangsters-opengl--samples-%23a90432)](https://github.com/erlangsters/opengl-samples)
![Supported Erlang/OTP Versions](https://img.shields.io/badge/erlang%2Fotp-28-%23a90432)
![License](https://img.shields.io/github/license/erlangsters/glfw)
[![Build Status](https://img.shields.io/github/actions/workflow/status/erlangsters/glfw/workflow.yml)](https://github.com/erlangsters/glfw/actions/workflows/workflow.yml)

This repository contains little OpenGL programs written in C with their
counter-part written in Erlang and Elixir. They can be used as reference to
write applications that use the the OpenGL
[bindings](https://github.com/orgs/erlangsters/repositories?q=opengl-), and
implicitly, the EGL [binding](https://github.com/erlangsters/egl-1.5). Note
that they use GLFW for the windowing aspect.

The available samples are the following.

- `colored-triangle` - Shows a basic colored triangle.
- `textured-cube` - Shows a rotating textured cube.

Each sample is written to work with OpenGL 3.3, OpenGL 4.6, OpenGL ES 2.0 and
OpenGL ES 3.2 (when possible). Note that OpenGL 4.6 on macOS is not available
and ANGLE was used for OpenGL ES on both macOS and Windows.

## OpenGL availability

- talk about libangle and how it enable OpenGL ES on macOS and Windows (it also
  provides EGL).

- talk about availability of OpenGL on macOS and Windows.

## Compile and run C samples

Samples can only be compiled for one OpenGL version at a time. Therefore, first
set the `OPENGL_VERSION` environment variable to one of the following values.

- `opengl-3.0`
- `opengl-4.6`
- `opengl-es-2.0`
- `opengl-es-3.2`

Then head to the `samples/c` folder and run `make`. All samples should be
compiled.

Run them individually. For instance `./hello-world`.

## Compile and run Erlang samples

Samples can only be compiled for one OpenGL version at a time. Therefore, first
set the `OPENGL_VERSION` environment variable to one of the following values.

- `opengl-3.0`
- `opengl-4.6`
- `opengl-es-2.0`
- `opengl-es-3.2`

Then head to the `samples/erlang` folder and run `make`. All samples should be
compiled.

Run them individually. For instance `./hello-world`.

## Available samples

Several samples are provided with each attempting to reproduce a major feature
of OpenGL. This section describes that.

### The 'hello-world' sample

To be written.

## Bar

To be written.

---

- OpenGL samples that I aim to have it ported to Erlang
- Help me showcase/test the code by having a reference



OpenGL ES 3.2

```
#include <stdio.h>
#include <stdlib.h>
#include <GLES3/gl3.h>
#include <EGL/egl.h>
#include <GLFW/glfw3.h>

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

CC = gcc
CFLAGS = -Wall -Wextra -I/usr/include
LDFLAGS = -lglfw -lGLESv2 -lEGL

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

```

OpenGL 4.6

```
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

CC = gcc
CFLAGS = -Wall -Wextra -I/usr/include
LDFLAGS = -lglfw -lGLEW -lGL

```


## Project origin

During the development of the EGL and OpenGL bindings for Erlang, references in
C needed to be used. Developing a large array of OpenGL bindings is challenging
as they have to be cross-platform and also because each version of OpenGL has
their specificities which we must test.

With those programs, we also set goals. Each program uses a specific feature of
OpenGL, and if we can get it to reproduce this exact same behavior in Erlang,
our task is complete.

In other words, they're sort of unit tests which allows test-driven
development.

Of course, those programs are excellent candidates for source of reference
after the bindings are complete and released to the public - They's why it's a
public [repo](https://github.com/erlangsters/opengl-samples).

## Files organization

You will find the C samples in the `samples/c/` folder and their Erlang
counter-parts in the `samples/erlang/` folder. Each folder comes with a
`Makefile` which allows you to conveniently compiles and run each sample.

Additionally, you will find the relevant quick reference cards as provided by
Khronos Group in the `cards/` folder.
