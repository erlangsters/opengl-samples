# OpenGL Samples Copilot Guidelines

- `opengl-samples` is an `example-reference` repository in the `graphics-stack` family and remains in active development.
- Preserve the repository's role as a small reference surface that shows how GLFW, EGL, and the OpenGL bindings fit together in native C and on the BEAM.
- Keep native samples in C for now. Do not switch them to C++ or introduce GLM in the native layer unless the repository direction changes intentionally.
- The native sample build lives in `samples/native/` and currently supports Linux only.
- Preserve the CMake model in `samples/native/CMakeLists.txt`: one version-group target per API version such as `opengl_46` or `opengl_es_31`, plus one executable target per sample and version combination.
- On Linux CI, keep the dependency story explicit: install `cmake`, `libegl-dev`, and `libglfw3-dev` for all native builds, then install `libgl-dev` for desktop OpenGL targets and `libgles-dev` for OpenGL ES targets.
- The native window path currently assumes X11 through `glfwGetX11Window()`. Do not present the native samples as portable to macOS or Windows until the native window handle path is implemented and verified there.
- Keep the layering clear in native code and docs: GLFW provides the window and event loop, EGL creates and binds the rendering context, and the OpenGL API varies by selected target version.
- When adjusting sample behavior, prefer changes that remain easy to mirror later in the Erlang and Elixir samples.