#include <string.h>
#include "util.h"
#include "config.h"

#ifdef WIN32
#include "vendor/pdcurses/curses.h"
#include <windows.h>
#else
#include <curses.h>
#endif

void initStartScreen() {
    #ifdef WIN32
        SetConsoleTitle(TEXT("Game of Life"));
    #endif
    initscr();
    nodelay(stdscr, TRUE);
    noecho();

    /* Hide Cursor */
    curs_set(0);

    /* Disable Scrolling in Console */
    scrollok (stdscr, TRUE);

    /* Add the Colors to the Console */
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_WHITE);
    init_pair(4, COLOR_RED, COLOR_WHITE);
    init_pair(5, COLOR_GREEN, COLOR_WHITE);
    init_pair(6, COLOR_BLUE, COLOR_WHITE);

    /* At first clear the Console */
    clear();
}

void updateHeadWin(WINDOW *headWin, GameOfLife GoL) {
    if(GoL.iteration == 0) {
        wbkgd(headWin, COLOR_PAIR(1));
        char appName[255];
        sprintf(appName, "Game of Life v%s", VERSION);
        waddstr(headWin, appName);
    }

    char interationString[255];
    sprintf(interationString, "%s:%i", "Interation", GoL.iteration);
    mvwaddstr(headWin, 0, COLS - strlen(interationString) -1, interationString);
    wrefresh(headWin);
}

void AusgabeSpielfeld(GameOfLife GoL, WINDOW* subWin){
    int o, i;
	for (o=0; o< GoL.settings.sizeY;o++){
	    for(i=0;i<GoL.settings.sizeX;i++){
            mvwaddch(subWin, o, i, GoL.currentIteration[o][i]);
	    }
    }

    wrefresh(subWin);
}
