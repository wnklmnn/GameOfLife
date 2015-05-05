#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "drawing.h"


#ifdef WIN32
#include "vendor/pdcurses/curses.h"
#else
#include <curses.h>
#endif

void ErstelleSpielfeld(GameOfLife *GoL);
void CalcIteration(GameOfLife *GoL);

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
