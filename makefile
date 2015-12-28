CC = gcc
CFLAGS = -g -Wall
DEFAULT_TARGET=GameOfLife
DEFAULT_OS=Linux

Default: $(DEFAULT_TARGET)_$(DEFAULT_OS)

GameOfLife_Linux: main.o drawing.o gol.o util.o
	$(CC) $(CFLAGS) -o GameOfLife main.o drawing.o gol.o util.o -l ncurses
GameOfLife_Windows: main.o drawing.o gol.o util.o
	$(CC) $(CFLAGS) -o GameOfLife main.o drawing.o gol.o util.o -l pdcurses -L vendor/pdcurses

util.o: util.c gol.h config.h util.h
	$(CC) $(CFLAGS) -c util.c
gol.o: gol.c gol.h
	$(CC) $(CFLAGS) -c gol.c
drawing.o: drawing.c util.h config.h
	$(CC) $(CFLAGS) -c drawing.c
main.o: main.c gol.h drawing.h util.h
	$(CC) $(CFLAGS) -c main.c
