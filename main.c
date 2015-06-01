#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "util.h"
#include "drawing.h"
#include "gol.h"


#ifdef WIN32
#include "vendor/pdcurses/curses.h"
#else
#include <curses.h>
#endif

int main(int argc, char** argv){
    struct timeval tv;
    double tmpTime1, tmpTime2;
    double timePerFrame;
    srand(time(NULL));
    GameOfLife GoL;
    GoL.lastTimestamp = (unsigned)time(NULL);

    if(readStartArguments(argc, argv, &GoL) == 1) {
        return 0;
    }
    timePerFrame = 1000 / GoL.settings.fps;

    initStartScreen();
    WINDOW *headWin = subwin(stdscr, 1, COLS, 0, 0);
    WINDOW *subWin = subwin(stdscr, LINES - 1, COLS, 1, 0);

    updateHeadWin(headWin, GoL);

    if(GoL.settings.importField == 1) {
        if(importSpielFeld(&GoL) == -1) {
            wmessage(subWin, "Import fehlgeschlagen.\nTippe eine beliebige Taste um zufälliges Spielfeld zu erstellen.\n");
            ErstelleSpielfeld(&GoL);
        }
    } else if(GoL.settings.exportField == 1 && GoL.settings.stepByStep == 'n') {
        wmessage(subWin, "Export abgebrochen: Export funktioniert nur beim Step by Step Mode\nTippe eine beliebige Taste um zufälliges Spielfeld zu erstellen.\n");
        ErstelleSpielfeld(&GoL);
    } else {
        ErstelleSpielfeld(&GoL);
    }

    while(1){
        gettimeofday(&tv, NULL);
        tmpTime1 = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000; 
        CalcIteration(&GoL);
        AusgabeSpielfeld(GoL, subWin);
        updateHeadWin(headWin, GoL);
        if (GoL.settings.stepByStep =='y'){
            if(GoL.settings.exportField == 1) {
                exportSpielFeld(&GoL);
            }
            getchar();
        }else {
        do{
            gettimeofday(&tv, NULL);
            tmpTime2 = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
            }while(tmpTime2 < tmpTime1+timePerFrame);
        }
    }

    return 0;
}
