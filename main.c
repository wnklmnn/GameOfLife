#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "util.h"
#include "drawing.h"
#include "gol.h"


#ifdef WIN32
#include "vendor/pdcurses/curses.h"
#else
#include <curses.h>
#endif

int main(int argc, char** argv){
    srand(time(NULL));
    GameOfLife GoL;

    if(readStartArguments(argc, argv, &GoL) == 1) {
        return 0;
    }

    initStartScreen();
    WINDOW *headWin = subwin(stdscr, 1, COLS, 0, 0);
    WINDOW *subWin = subwin(stdscr, LINES - 1, COLS, 1, 0);

    updateHeadWin(headWin, GoL);

	//Initialize Array
    ErstelleSpielfeld(&GoL);

    while(1){
        CalcIteration(&GoL);
        AusgabeSpielfeld(GoL, subWin);
        updateHeadWin(headWin, GoL);
        usleep(50000);
    }

    return 0;
}
