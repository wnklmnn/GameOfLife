GCC= gcc -Wall -Werror -g
FILES = util.c main.c

linux:
	$(GCC) $(FILES) -o GameOfLife -lcurses

windows:
	$(GCC) $(FILES) -L "vendor/pdcurses" -lpdcurses -o GameOfLife

clean:
	rm GameOfLife*