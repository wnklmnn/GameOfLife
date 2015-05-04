#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "util.h"
#include <curses.h>

void AusgabeSpielfeld(struct GameOfLife GoL);
void ErstelleSpielfeld(struct GameOfLife *GoL);
void CalcIteration(struct GameOfLife *GoL);

int main(int argc, char** argv){
    initscr();
    srand(time(NULL));
    struct GameOfLife GoL;

    if(readStartArguments(argc, argv, &GoL) == 1) {
        return 0;
    }

	//Initialize Array
    ErstelleSpielfeld(&GoL);
	
	
    while(1==1){
        CalcIteration(&GoL);
        AusgabeSpielfeld(GoL);
        getchar();
    }	
    
    return 0;
}

void CalcIteration(struct GameOfLife *GoL){
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

void AusgabeSpielfeld(struct GameOfLife GoL){
    int o, i;
    mvprintw(0, 0,"Ausgabe Iteration: %i %i*%i\n",GoL.iteration, GoL.settings.sizeY, GoL.settings.sizeX);
	for (o=0; o< GoL.settings.sizeY;o++){
	    for(i=0;i<GoL.settings.sizeX;i++){
	        mvprintw(o+1, i,"%c", GoL.currentIteration[o][i]);
	    }
	    printw("\n");
    }
    refresh();
}

void ErstelleSpielfeld(struct GameOfLife *GoL){
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
