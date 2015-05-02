
GCC= gcc -Wall -Werror -g

linux:
	$(GCC) main.c -o GameOfLife

windows:
	$(GCC) main.c -o GameOfLife

clean:
	rm GameOfLife*