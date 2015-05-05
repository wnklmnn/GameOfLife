#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include <curses.h>

#ifdef WIN32
#include "vendor/pdcurses/curses.h"
#include <windows.h>
#else
#include <curses.h>
#endif

void AusgabeSpielfeld(GameOfLife GoL, WINDOW* subWin);
void ErstelleSpielfeld(GameOfLife *GoL);
void CalcIteration(GameOfLife *GoL);
void initStartScreen();
void updateHeadWin(WINDOW *headWin, GameOfLife GoL);

int main(int argc, char** argv){
    #ifdef WIN32
        SetConsoleTitle(TEXT("Game of Life"));
    #endif
    srand(time(NULL));
    GameOfLife GoL;

    if(readStartArguments(argc, argv, &GoL) == 1) {
        return 0;
    }

    initStartScreen();
    WINDOW *headWin = subwin(stdscr, 1, COLS, 0, 0);
    WINDOW *subWin = subwin ( stdscr, LINES - 1, COLS, 1, 0 );
    wbkgd(headWin, COLOR_PAIR(1));

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

void CalcIteration(GameOfLife *GoL){
    int o, i;
    int aliveNeightbors;
    char **tmpIteration;
    for (o=0; o<GoL->settings.sizeY; o++){
        for(i=0; i<GoL->settings.sizeX; i++){
        GoL->nextIteration[o][i] = GoL->settings.deadCellChar;
        //zaehlen der Lebenden Nachbarn je nach einstellung
            if (GoL->settings.edgeBehavior==0){ // Ausßerhalb sind Tote Zellen
                aliveNeightbors = 0;
                //OL
                aliveNeightbors += ( o>0 && i > 0 && GoL->currentIteration[o-1][i-1]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //O
                aliveNeightbors += ( o>0 && GoL->currentIteration[o-1][i]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //OR
                aliveNeightbors += ( o>0 && i < GoL->settings.sizeX && GoL->currentIteration[o-1][i+1]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //L
                aliveNeightbors += ( i > 0 && GoL->currentIteration[o][i-1]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //R
                aliveNeightbors += ( i < GoL->settings.sizeX-1  && GoL->currentIteration[o][i+1]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //UL
                aliveNeightbors += ( o<GoL->settings.sizeY-1 && i > 0 && GoL->currentIteration[o+1][i-1]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //U
                aliveNeightbors += ( o < GoL->settings.sizeY-1 && GoL->currentIteration[o+1][i]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //UR
                aliveNeightbors += ( o<GoL->settings.sizeY-1 && i < GoL->settings.sizeX && GoL->currentIteration[o+1][i+1]==GoL->settings.aliveCellChar ) ? 1 : 0;
            }else if (GoL->settings.edgeBehavior == 1){ //Ausßerhalb sind Lebende Zellen

            }else if (GoL->settings.edgeBehavior == 2){ // Das spielfeld ist Kugelförmig.
                aliveNeightbors = 0;
                //OL
                aliveNeightbors += ( GoL->currentIteration[((o-1) % GoL->settings.sizeY + GoL->settings.sizeY) % GoL->settings.sizeY][((i-1) % GoL->settings.sizeX + GoL->settings.sizeX) % GoL->settings.sizeX]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //O
                aliveNeightbors += ( GoL->currentIteration[((o-1) % GoL->settings.sizeY + GoL->settings.sizeY) % GoL->settings.sizeY][((i) % GoL->settings.sizeX + GoL->settings.sizeX) % GoL->settings.sizeX]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //OR
                aliveNeightbors += ( GoL->currentIteration[((o-1) % GoL->settings.sizeY + GoL->settings.sizeY) % GoL->settings.sizeY][((i+1) % GoL->settings.sizeX + GoL->settings.sizeX) % GoL->settings.sizeX]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //L
                aliveNeightbors += ( GoL->currentIteration[((o) % GoL->settings.sizeY + GoL->settings.sizeY) % GoL->settings.sizeY][((i-1) % GoL->settings.sizeX + GoL->settings.sizeX) % GoL->settings.sizeX]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //R
                aliveNeightbors += ( GoL->currentIteration[((o) % GoL->settings.sizeY + GoL->settings.sizeY) % GoL->settings.sizeY][((i+1) % GoL->settings.sizeX + GoL->settings.sizeX) % GoL->settings.sizeX]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //UL
                aliveNeightbors += ( GoL->currentIteration[((o+1) % GoL->settings.sizeY + GoL->settings.sizeY) % GoL->settings.sizeY][((i-1) % GoL->settings.sizeX + GoL->settings.sizeX) % GoL->settings.sizeX]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //U
                aliveNeightbors += ( GoL->currentIteration[((o+1) % GoL->settings.sizeY + GoL->settings.sizeY) % GoL->settings.sizeY][((i) % GoL->settings.sizeX + GoL->settings.sizeX) % GoL->settings.sizeX]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //UR
                aliveNeightbors += ( GoL->currentIteration[((o+1) % GoL->settings.sizeY + GoL->settings.sizeY) % GoL->settings.sizeY] [((i+1) % GoL->settings.sizeX + GoL->settings.sizeX) % GoL->settings.sizeX]==GoL->settings.aliveCellChar ) ? 1 : 0;
            }
        //Lebt oder stirbt die Zelle in der nächsten Generation?
        //Hier könnte man noch verschiedene Regeln einbauen?
            if(GoL->currentIteration[o][i] == GoL->settings.aliveCellChar && aliveNeightbors < 2){
                GoL->nextIteration[o][i] = GoL->settings.deadCellChar;
            }
            if (GoL->currentIteration[o][i] == GoL->settings.deadCellChar && aliveNeightbors == 3){
                GoL->nextIteration[o][i] = GoL->settings.aliveCellChar;
            }
            if (GoL->currentIteration[o][i] == GoL->settings.aliveCellChar && (aliveNeightbors == 2 || aliveNeightbors == 3)){
                GoL->nextIteration[o][i] = GoL->settings.aliveCellChar;
            }
            if(GoL->currentIteration[o][i] == GoL->settings.aliveCellChar && aliveNeightbors > 3){
                GoL->nextIteration[o][i] = GoL->settings.deadCellChar;
            }

        }
    }
    tmpIteration = GoL->currentIteration;
    GoL->currentIteration = GoL->nextIteration;
    GoL->nextIteration = tmpIteration;
    GoL->iteration += 1;
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

void ErstelleSpielfeld(GameOfLife *GoL){
    int i, o;
    GoL->currentIteration = malloc(GoL->settings.sizeY * sizeof(char*));
	for(i=0;i<GoL->settings.sizeY;i++){
        GoL->currentIteration[i] = malloc(GoL->settings.sizeX * sizeof(char));
	}

	GoL->nextIteration = malloc(GoL->settings.sizeY * sizeof(char*));
	for(i=0;i<GoL->settings.sizeY;i++){
        GoL->nextIteration[i] = malloc(GoL->settings.sizeX * sizeof(char));
	}

	for(o=0;o<GoL->settings.sizeY;o++){
	    for(i=0;i<GoL->settings.sizeX;i++){
	        GoL->currentIteration[o][i] = rand()%2 ? GoL->settings.aliveCellChar : GoL->settings.deadCellChar;
	    }
	}
}

void initStartScreen() {
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
        waddstr(headWin, "Game of Life v 0.01");
    }

    char interationString[255];
    sprintf(interationString, "%s:%i", "Interation", GoL.iteration);
    mvwaddstr(headWin, 0, COLS - strlen(interationString) -1, interationString);
    wrefresh(headWin);
}
