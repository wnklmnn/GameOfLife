#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "gol.h"

void CalcIteration(GameOfLife *GoL){
    if((unsigned)time(NULL) == GoL->lastTimestamp) {
        GoL->interationCount++;
    } else {
        GoL->interationPerSecond = GoL->interationCount;
        GoL->interationCount = 1;
        GoL->lastTimestamp = (unsigned)time(NULL);
    }

    int o, i, k;
    int aliveNeightbors;
    int cellStillAlive;
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
                aliveNeightbors += ( o>0 && i < GoL->settings.sizeX -1 && GoL->currentIteration[o-1][i+1]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //L
                aliveNeightbors += ( i > 0 && GoL->currentIteration[o][i-1]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //R
                aliveNeightbors += ( i < GoL->settings.sizeX-1  && GoL->currentIteration[o][i+1]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //UL
                aliveNeightbors += ( o<GoL->settings.sizeY-1 && i > 0 && GoL->currentIteration[o+1][i-1]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //U
                aliveNeightbors += ( o < GoL->settings.sizeY-1 && GoL->currentIteration[o+1][i]==GoL->settings.aliveCellChar ) ? 1 : 0;
                //UR
                aliveNeightbors += ( o<GoL->settings.sizeY-1 && i < GoL->settings.sizeX -1 && GoL->currentIteration[o+1][i+1]==GoL->settings.aliveCellChar ) ? 1 : 0;
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
        cellStillAlive = 0;
         if (GoL->currentIteration[o][i]==GoL->settings.aliveCellChar){
             for (k=0; k<strlen(GoL->settings.rule_aliveNumber); k++){
                if (aliveNeightbors == (GoL->settings.rule_aliveNumber[k])-'0'){
                    GoL->nextIteration[o][i] = GoL->settings.aliveCellChar;
                    cellStillAlive = 1;
                    break;
                }
             }
         }
         if (GoL->currentIteration[o][i]==GoL->settings.deadCellChar){
             for (k=0;k<strlen(GoL->settings.rule_birthNumber); k++){
                if (aliveNeightbors == ((GoL->settings.rule_birthNumber[k])-'0')){
                    GoL->nextIteration[o][i] = GoL->settings.aliveCellChar;
                    cellStillAlive = 1;
                    break;
                }
             }
         }
         if(cellStillAlive==0){
            GoL->nextIteration[o][i] = GoL->settings.deadCellChar;
         }
        //Hier könnte man noch verschiedene Regeln einbauen?
/*            if(GoL->currentIteration[o][i] == GoL->settings.aliveCellChar && aliveNeightbors < 2){
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
*/
        }
    }
    tmpIteration = GoL->currentIteration;
    GoL->currentIteration = GoL->nextIteration;
    GoL->nextIteration = tmpIteration;
    //Kopieren des Spielfeldes in den bereich der Vergangenen Spielfelder

    for(o=0; o<GoL->settings.sizeY; o++){
        memcpy(GoL->pastIterations[GoL->iteration%GoL->settings.numberOfPastIterations][o], tmpIteration[o], sizeof(char)*GoL->settings.sizeX);
    }
    GoL->iteration += 1;
}

int  UeberpruefeSpielfeldAufLoop(GameOfLife GoL){
    int i, o, k;
    int theSame;
    for(k=0;k<GoL.settings.numberOfPastIterations;k++){
        theSame = 1;
        for(o=0 ;o < GoL.settings.sizeY ;o++){
	        for(i=0;i<GoL.settings.sizeX;i++){
	            if (GoL.currentIteration[o][i] != GoL.pastIterations[k][o][i]){
	                theSame = 0;
                    break; 
	            }
	            
	        }
	        if (theSame==0){
                break;
            }
	    }
	    if(theSame==1){
            return 1;
        }
	}
	return 0;
}


void ErstellePastIterations(GameOfLife *GoL){
    int i,k;
    GoL->pastIterations = malloc(GoL->settings.numberOfPastIterations * sizeof(char*));
    for(k=0;k<GoL->settings.numberOfPastIterations;k++){        
        GoL->pastIterations[k] = malloc(GoL->settings.sizeY * sizeof(char*));
	    for(i=0;i<GoL->settings.sizeY;i++){
            GoL->pastIterations[k][i] = malloc(GoL->settings.sizeX * sizeof(char));
	    }
        /*for(o=0; o<GoL->settings.sizeY; o++){
            for(i=0; i<GoL->settings.sizeX; i++){
                GoL->pastIterations[k][o][i] = '\0';
            }
        }*/
    }
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
