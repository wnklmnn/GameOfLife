#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
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

    if(GoL.settings.importField == 1) {
        if(importSpielFeld(&GoL) == -1) {
            waddstr(subWin, "Import fehlgeschlagen.\nTippe eine beliebige Taste um zufälliges Spielfeld zu erstellen.\n");
            wrefresh(subWin);
            getchar();
            wclear(subWin);
            wrefresh(subWin);
            ErstelleSpielfeld(&GoL);
        }
    } else {
        ErstelleSpielfeld(&GoL);
    }

    while(1){
        CalcIteration(&GoL);
        AusgabeSpielfeld(GoL, subWin);
        updateHeadWin(headWin, GoL);
        if (GoL.settings.stepByStep =='y'){
            getchar();
        }else {
            usleep(50000);
        }
    }

    return 0;
}
