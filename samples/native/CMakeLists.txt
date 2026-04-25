cmake_minimum_required(VERSION 3.10)
project(opengl_samples_native LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(glfw3 CONFIG QUIET)

if(TARGET glfw)
    set(GLFW_LINK_TARGET glfw)
elseif(TARGET glfw3)
    set(GLFW_LINK_TARGET glfw3)
elseif(TARGET glfw3::glfw)
    set(GLFW_LINK_TARGET glfw3::glfw)
elseif(TARGET glfw3::glfw3)
    set(GLFW_LINK_TARGET glfw3::glfw3)
else()
    find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h REQUIRED)
    find_library(GLFW_LIBRARY NAMES glfw3 glfw REQUIRED)
endif()

if(APPLE OR WIN32)
    if(NOT DEFINED ANGLE_INCLUDE_DIR)
        message(FATAL_ERROR "ANGLE_INCLUDE_DIR must be defined (pass -DANGLE_INCLUDE_DIR=...)")
    endif()
    if(NOT DEFINED ANGLE_LIB_DIR)
        message(FATAL_ERROR "ANGLE_LIB_DIR must be defined (pass -DANGLE_LIB_DIR=...)")
    endif()

    set(EGL_INCLUDE_DIR "${ANGLE_INCLUDE_DIR}")
    set(GLES2_INCLUDE_DIR "${ANGLE_INCLUDE_DIR}")
    find_library(EGL_LIBRARY NAMES EGL libEGL REQUIRED PATHS ${ANGLE_LIB_DIR} NO_DEFAULT_PATH)
    find_library(GLES2_LIBRARY NAMES GLESv2 libGLESv2 REQUIRED PATHS ${ANGLE_LIB_DIR} NO_DEFAULT_PATH)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_path(EGL_INCLUDE_DIR EGL/egl.h REQUIRED)
    find_library(EGL_LIBRARY NAMES EGL REQUIRED)

    find_path(OPENGL_INCLUDE_DIR GL/gl.h REQUIRED)
    find_library(OPENGL_LIBRARY NAMES GL OpenGL REQUIRED)

    find_path(GLES2_INCLUDE_DIR GLES2/gl2.h REQUIRED)
    find_library(GLES2_LIBRARY NAMES GLESv2 REQUIRED)

    find_library(MATH_LIBRARY NAMES m REQUIRED)
else()
    message(FATAL_ERROR "Unsupported platform for the native samples.")
endif()

set(COMMON_SOURCES
    src/common/matrix.c
    src/common/window.c
)

set(COMMON_INCLUDE_DIRS
    ${EGL_INCLUDE_DIR}
    ${GLES2_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/common
)

if(DEFINED GLFW_INCLUDE_DIR)
    list(APPEND COMMON_INCLUDE_DIRS ${GLFW_INCLUDE_DIR})
endif()

if(DEFINED OPENGL_INCLUDE_DIR)
    list(APPEND COMMON_INCLUDE_DIRS ${OPENGL_INCLUDE_DIR})
endif()

set(COMMON_LINK_LIBRARIES
    ${EGL_LIBRARY}
)

if(DEFINED GLFW_LINK_TARGET)
    list(APPEND COMMON_LINK_LIBRARIES ${GLFW_LINK_TARGET})
else()
    list(APPEND COMMON_LINK_LIBRARIES ${GLFW_LIBRARY})
endif()

if(DEFINED MATH_LIBRARY)
    list(APPEND COMMON_LINK_LIBRARIES ${MATH_LIBRARY})
endif()

set(SAMPLES
    colored-triangle
    textured-cube
)

macro(add_native_samples_for_version group_target version_name version_macro api_kind)
    set(${group_target}_targets "")

    foreach(sample IN LISTS SAMPLES)
        string(REPLACE "-" "_" sample_id "${sample}")
        string(REPLACE "." "_" version_id "${version_name}")
        string(REPLACE "-" "_" version_id "${version_id}")

        set(target_name "${sample_id}_${version_id}")

        add_executable(${target_name}
            src/${sample}/main.c
            ${COMMON_SOURCES}
        )

        target_include_directories(${target_name} PRIVATE ${COMMON_INCLUDE_DIRS})
        target_compile_options(${target_name} PRIVATE -Wall -Wextra)
        target_compile_definitions(${target_name} PRIVATE ${version_macro})
        set_target_properties(${target_name} PROPERTIES OUTPUT_NAME "${sample}-${version_name}")

        if("${api_kind}" STREQUAL "gl")
            target_link_libraries(${target_name} PRIVATE ${COMMON_LINK_LIBRARIES} ${OPENGL_LIBRARY})
        elseif("${api_kind}" STREQUAL "gles")
            target_link_libraries(${target_name} PRIVATE ${COMMON_LINK_LIBRARIES} ${GLES2_LIBRARY})
        else()
            message(FATAL_ERROR "Unsupported API kind: ${api_kind}")
        endif()

        list(APPEND ${group_target}_targets ${target_name})
    endforeach()

    add_custom_target(${group_target} DEPENDS ${${group_target}_targets})
endmacro()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_native_samples_for_version(opengl_33 "opengl-3.3" OPENGL_VERSION_33 gl)
    add_native_samples_for_version(opengl_41 "opengl-4.1" OPENGL_VERSION_41 gl)
    add_native_samples_for_version(opengl_46 "opengl-4.6" OPENGL_VERSION_46 gl)
    add_native_samples_for_version(opengl_es_20 "opengl-es-2.0" OPENGL_ES_VERSION_20 gles)
    add_native_samples_for_version(opengl_es_30 "opengl-es-3.0" OPENGL_ES_VERSION_30 gles)
    add_native_samples_for_version(opengl_es_31 "opengl-es-3.1" OPENGL_ES_VERSION_31 gles)
    add_native_samples_for_version(opengl_es_32 "opengl-es-3.2" OPENGL_ES_VERSION_32 gles)
elseif(APPLE OR WIN32)
    add_native_samples_for_version(opengl_es_31 "opengl-es-3.1" OPENGL_ES_VERSION_31 gles)
endif()