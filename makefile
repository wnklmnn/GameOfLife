GCC= gcc -Wall -Werror -g
FILES = util.c main.c drawing.c

linux:
	$(GCC) $(FILES) -o GameOfLife -lncurses

windows:
	$(GCC) $(FILES) -L "vendor/pdcurses" -lpdcurses -o GameOfLife

clean:
	rm GameOfLife*
