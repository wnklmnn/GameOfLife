
GCC= gcc -Wall -Werror -g

GameOfLife: main.o
	$(GCC) main.o -o GameOfLife

main.o: main.c
	$(GCC) -c main.c

