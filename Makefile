CC = gcc
CFLAGS = -Wall -I./samples/common
LDFLAGS = -lglfw -lEGL -lGLESv2

COMMON_SRC = samples/common/window.c

.PHONY: all clean

all: hello-world foo bar

hello-world: samples/hello-world/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

foo: samples/foo/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

bar: samples/bar/main.c $(COMMON_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f hello-world foo bar
