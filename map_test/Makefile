CC = gcc
LIBS = -lSDL2 -lSDL2_image
INCLUDE=-I/usr/include/SDL2

build : src/main.c src/map.c
	$(CC) -g $(INCLUDE) $^ -o bin/game $(LIBS)

run: build
	bin/game
