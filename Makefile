CC=gcc
CFLAGS=-O3 -std=c99 -Wall -Wno-unused-function $(shell pkg-config --cflags sdl2 gl openal glew)
LDFLAGS=$(shell pkg-config --libs sdl2 gl openal glew) -lm
TARGET=q2
SRC=q2.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f $(TARGET)

test: $(TARGET)
	./$(TARGET)

.PHONY: all clean test
