CC = gcc
LIBS = -lSDL2 -lSDL2_image
INCLUDE = 

build : src/main.c src/map.c
	$(CC) -g $(INCLUDE) $^ -o bin/game $(LIBS)

run: build
	bin/game
