CC = gcc
CFLAGS = -Wall -I./src/common
LDFLAGS = -lglfw -lEGL -lGLESv2 -lm

COMMON_SRC = src/common/matrix.c src/common/window.c

.PHONY: all clean

all: colored-triangle textured-cube

colored-triangle: src/colored-triangle/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

textured-cube: src/textured-cube/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f colored-triangle textured-cube
