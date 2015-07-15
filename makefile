GCC= gcc -Wall -Werror -g
FILES = util.c main.c drawing.c gol.c

linux:
	$(GCC) $(FILES) -o GameOfLife `pkgconf --cflags -libs ncurses`

windows:
	$(GCC) $(FILES) -L "vendor/pdcurses" -lpdcurses -o GameOfLife

clean:
	rm GameOfLife*
