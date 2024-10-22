CC = gcc
CFLAGS = -Wall -I./src/common
LDFLAGS = -lglfw -lEGL -lGLESv2

COMMON_SRC = src/common/window.c

.PHONY: all clean

all: hello-world foo bar

hello-world: src/hello-world/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

foo: src/foo/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bar: src/bar/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f hello-world foo bar
