GCC= gcc -Wall -Werror -g
FILES = util.c main.c

linux:
	$(GCC) $(FILES) -o GameOfLife -lncurses

windows:
	$(GCC) $(FILES) -o GameOfLife

clean:
	rm GameOfLife*
