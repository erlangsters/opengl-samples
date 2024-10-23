OPENGL_VERSION ?= opengl-4.6
COMMON_SRC = src/common/matrix.c src/common/window.c

CC = gcc
CFLAGS = -Wall -I./src/common

LDFLAGS = -lglfw -lEGL
ifeq ($(OPENGL_VERSION), opengl-3.3)
    CFLAGS += -DOPENGL_VERSION_33
    LDFLAGS += -lGL
else ifeq ($(OPENGL_VERSION), opengl-4.6)
    CFLAGS += -DOPENGL_VERSION_46
    LDFLAGS += -lGL
else ifeq ($(OPENGL_VERSION), opengl-es-2.0)
    CFLAGS += -DOPENGL_ES_VERSION_20
    LDFLAGS += -lGLESv2
else ifeq ($(OPENGL_VERSION), opengl-es-3.2)
    CFLAGS += -DOPENGL_ES_VERSION_32
    LDFLAGS += -lGLESv3
else
    $(error Unsupported OpenGL version. Please set OPENGL_VERSION to one of: opengl-3.3, opengl-4.6, opengl-es-2.0, opengl-es-3.2)
endif
LDFLAGS += -lm

.PHONY: all clean

all: colored-triangle textured-cube

colored-triangle: src/colored-triangle/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

textured-cube: src/textured-cube/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f colored-triangle textured-cube
