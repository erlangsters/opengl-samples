CC = gcc
CFLAGS = -Wall -I./src/common
LDFLAGS = -lglfw -lEGL -lGLESv2

COMMON_SRC = src/common/window.c

.PHONY: all clean

all: colored-triangle foo bar

colored-triangle: src/colored-triangle/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

foo: src/foo/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bar: src/bar/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f colored-triangle foo bar
