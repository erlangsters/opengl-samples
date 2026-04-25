# OpenGL Samples (Erlang & Elixir)

> :construction: It's a work-in-progress. Notably, the Elixir examples are not
> implemented yet.

[![Erlangsters Repository](https://img.shields.io/badge/erlangsters-opengl--samples-%23a90432)](https://github.com/erlangsters/opengl-samples)
![Supported Erlang/OTP Versions](https://img.shields.io/badge/erlang%2Fotp-28-%23a90432)
![License](https://img.shields.io/github/license/erlangsters/glfw)
[![Build Status](https://img.shields.io/github/actions/workflow/status/erlangsters/glfw/workflow.yml)](https://github.com/erlangsters/glfw/actions/workflows/workflow.yml)

This repository contains simple OpenGL programs written in C with their
counter-part written in Erlang and Elixir. They can be used as reference to
write applications that use the the OpenGL
[bindings](https://github.com/orgs/erlangsters/repositories?q=opengl-), and
implicitly, the EGL [binding](https://github.com/erlangsters/egl-1.5). Note
that they use GLFW to display the graphics in a window.

The available samples are the following.

- `colored-triangle` - Shows a basic colored triangle.
- `textured-cube` - Shows a rotating textured cube.

Each sample is written to work with all OpenGL and OpenGL ES versions that are
made available to Erlang and Elixir.

- OpenGL: [3.3](https://github.com/erlangsters/opengl-3.3) |
  [4.1](https://github.com/erlangsters/opengl-4.1) |
  [4.6](https://github.com/erlangsters/opengl-4.6)
- OpenGL ES: [2.0](https://github.com/erlangsters/opengl-es-2.0) |
  [3.0](https://github.com/erlangsters/opengl-es-3.0) |
  [3.1](https://github.com/erlangsters/opengl-es-3.1) |
  [3.2](https://github.com/erlangsters/opengl-es-3.2)

On macOS and Windows, the native samples currently target OpenGL ES 3.1
through the ANGLE layer. Linux remains the platform where the wider desktop
OpenGL and OpenGL ES matrix is compiled and exercised.

## OpenGL availability

All versions of OpenGL and OpenGL ES are available on Linux, and work is primarily
tested on this platform.

However, the **macOS** and **Windows** platforms have limitations.

On both platforms, the current native sample build uses ANGLE to provide EGL and
an OpenGL ES implementation on top of the host system. That keeps the sample
layer aligned with the EGL-first architecture used by the bindings.

With that said, Linux is still the broadest verification platform. The macOS and
Windows jobs currently build the OpenGL ES 3.1 samples only.

## Compile and run C samples

The native samples now use CMake.

Instead of reconfiguring the same build tree with an environment variable, CMake exposes one target per OpenGL version and one executable target per sample/version pair.

The version group targets are.

- `opengl_33`
- `opengl_41`
- `opengl_46`
- `opengl_es_20`
- `opengl_es_30`
- `opengl_es_31`
- `opengl_es_32`

Configure the project once, then build the version you want.

```
cmake -S samples/native -B samples/native/build
cmake --build samples/native/build --target opengl_es_31
```

This produces both native samples for OpenGL ES 3.1 in the build directory.

If you want a single executable, build its sample-specific target. For instance, to build only the `colored-triangle` sample for OpenGL 4.6:

```
cmake --build samples/native/build --target colored_triangle_opengl_4_6
./samples/native/build/colored-triangle-opengl-4.6
```

On macOS and Windows, configure the native samples with ANGLE and the vcpkg
toolchain, then build the `opengl_es_31` target.

```sh
cmake -S samples/native -B samples/native/build \
  -DCMAKE_TOOLCHAIN_FILE="$VCPKG_INSTALLATION_ROOT/scripts/buildsystems/vcpkg.cmake" \
  -DANGLE_INCLUDE_DIR="$ANGLE_INCLUDE_DIR" \
  -DANGLE_LIB_DIR="$ANGLE_LIB_DIR"
cmake --build samples/native/build --target opengl_es_31
```

For the moment, `opengl_es_31` is the non-Linux target that is wired into CI.

## Compile and run Erlang samples

Samples can only be compiled for one OpenGL version at a time. However, unlike
the C samples, all samples are compiled at the same time with a single command.

```
rebar3 as opengl_es_2.0 compile
```

To run a specific sample, use.

```
rebar3 as opengl_es_2.0 shell --eval 'colored_triangle:run().'
```


Therefore, first
set the `OPENGL_VERSION` environment variable to one of the following values.

- `opengl-3.0`
- `opengl-4.6`
- `opengl-es-2.0`
- `opengl-es-3.2`

Then head to the `samples/erlang` folder and run `make`. All samples should be
compiled.

Run them individually. For instance `./hello-world`.

## Compile and run Elixir samples

XXX: Not implemented yet.

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
