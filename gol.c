#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "gol.h"

/*
    In dieser Funktion wir die naechste Iteration berrechnet und die Zeiger aud die aktuelle und naechste Iteration werden getauscht
*/
void CalcIteration(GameOfLife* GoL)
{
    if((unsigned)time(NULL) == GoL->lastTimestamp) {
        GoL->interationCount++;
    } else {
        GoL->interationPerSecond = GoL->interationCount;
        GoL->interationCount = 1;
        GoL->lastTimestamp = (unsigned)time(NULL);
    }

    int o, i, k, outer, inner;
    int aliveNeightbors;
    int cellStillAlive;
    char** tmpIteration;
    for (o = 0; o < GoL->settings.sizeY; o++) {
        for(i = 0; i < GoL->settings.sizeX; i++) {
            GoL->nextIteration[o][i] = GoL->settings.deadCellChar;
            //zaehlen der Lebenden Nachbarn je nach einstellung
            if (GoL->settings.edgeBehavior == 0) { // Ausßerhalb sind Tote Zellen
                aliveNeightbors = 0;
                for(outer = -1; outer < 2; outer++) {
                    for(inner = -1; inner < 2; inner++) {
                        if(outer == 0 && inner == 0) {
                            continue;
                        }
                        if((o + outer) < 0 || (o + outer) > GoL->settings.sizeY - 1 || (i + inner) < 0 || (i + inner) > GoL->settings.sizeX - 1) {
                            continue;
                        } else {
                            aliveNeightbors += (GoL->currentIteration[o + outer ][i + inner] == GoL->settings.aliveCellChar ) ? 1 : 0;
                        }
                    }
                }
            } else if (GoL->settings.edgeBehavior == 1) { //Ausßerhalb sind Lebende Zellen
                aliveNeightbors = 0;
                for(outer = -1; outer < 2; outer++) {
                    for(inner = -1; inner < 2; inner++) {
                        if(outer == 0 && inner == 0) {
                            continue;
                        }
                        if((o + outer) < 0 || (o + outer) > GoL->settings.sizeY - 1 || (i + inner) < 0 || (i + inner) > GoL->settings.sizeX - 1) {
                            aliveNeightbors ++;
                            continue;
                        } else {
                            aliveNeightbors += (GoL->currentIteration[o + outer ][i + inner] == GoL->settings.aliveCellChar ) ? 1 : 0;
                        }
                    }
                }
            } else if (GoL->settings.edgeBehavior == 2) { // Das spielfeld ist Kugelförmig.
                aliveNeightbors = 0;
                for(outer = -1; outer < 2; outer++) {
                    for(inner = -1; inner < 2; inner++) {
                        if(outer == 0 && inner == 0) {
                            continue;
                        }
                        aliveNeightbors += ( GoL->currentIteration[((o + outer) % GoL->settings.sizeY + GoL->settings.sizeY) % GoL->settings.sizeY][((i + inner) % GoL->settings.sizeX + GoL->settings.sizeX) % GoL->settings.sizeX] == GoL->settings.aliveCellChar ) ? 1 : 0;
                    }
                }
            }
            //Lebt oder stirbt die Zelle in der nächsten Generation?
            /*
                In diesem Bereich wird der Zustand der Zellen festgelegt anhand der Regeln welche in GoL->settings.rule_aliveNumber und GoL->settings.rule_birthNumber hinterlegt sind
            */
            cellStillAlive = 0;
            if (GoL->currentIteration[o][i] == GoL->settings.aliveCellChar) {
                for (k = 0; k < strlen(GoL->settings.rule_aliveNumber); k++) {
                    if (aliveNeightbors == (GoL->settings.rule_aliveNumber[k]) - '0') {
                        GoL->nextIteration[o][i] = GoL->settings.aliveCellChar;
                        cellStillAlive = 1;
                        break;
                    }
                }
            }
            if (GoL->currentIteration[o][i] == GoL->settings.deadCellChar) {
                for (k = 0; k < strlen(GoL->settings.rule_birthNumber); k++) {
                    if (aliveNeightbors == ((GoL->settings.rule_birthNumber[k]) - '0')) {
                        GoL->nextIteration[o][i] = GoL->settings.aliveCellChar;
                        cellStillAlive = 1;
                        break;
                    }
                }
            }
            if(cellStillAlive == 0) {
                GoL->nextIteration[o][i] = GoL->settings.deadCellChar;
            }
        }
    }
    tmpIteration = GoL->currentIteration;
    GoL->currentIteration = GoL->nextIteration;
    GoL->nextIteration = tmpIteration;

    /*
        Kopieren des Spielfeldes in den bereich der Vergangenen Spielfelder
    */
    for(o = 0; o < GoL->settings.sizeY; o++) {
        memcpy(GoL->pastIterations[GoL->iteration % GoL->settings.numberOfPastIterations][o], tmpIteration[o], sizeof(char)*GoL->settings.sizeX);
    }
    GoL->iteration += 1;
}

/*
    In dieser Funktion wird durch die Vergangenen Iterationen iteriert und ueberprueft ob die aktuelle Iteration schon einmal vorhanden war.
    Wenn dies der Fall ist dann ist ein stabiler Zustand erreicht.
*/
int  UeberpruefeSpielfeldAufLoop(GameOfLife GoL)
{
    int i, o, k;
    int theSame;
    for(k = 0; k < GoL.settings.numberOfPastIterations; k++) {
        theSame = 1;
        for(o = 0 ; o < GoL.settings.sizeY ; o++) {
            for(i = 0; i < GoL.settings.sizeX; i++) {
                if (GoL.currentIteration[o][i] != GoL.pastIterations[k][o][i]) {
                    theSame = 0;
                    break;
                }

            }
            if (theSame == 0) {
                break;
            }
        }
        if(theSame == 1) {
            return 1;
        }
    }
    return 0;
}

/*
    In dieser Funktion wird Speicher allokiert um die vergangenen Iterationen zu speichern
*/
void ErstellePastIterations(GameOfLife* GoL)
{
    int i, k;
    GoL->pastIterations = malloc(GoL->settings.numberOfPastIterations * sizeof(char*));
    for(k = 0; k < GoL->settings.numberOfPastIterations; k++) {
        GoL->pastIterations[k] = malloc(GoL->settings.sizeY * sizeof(char*));
        for(i = 0; i < GoL->settings.sizeY; i++) {
            GoL->pastIterations[k][i] = malloc(GoL->settings.sizeX * sizeof(char));
        }
        /*for(o=0; o<GoL->settings.sizeY; o++){
            for(i=0; i<GoL->settings.sizeX; i++){
                GoL->pastIterations[k][o][i] = '\0';
            }
        }*/
    }
}


/*
    In dieser Funktion wird Speicher fuer die aktuelle und die naechste Iteration allokiert.
*/
void ErstelleSpielfeld(GameOfLife* GoL)
{
    int i, o;
    GoL->currentIteration = malloc(GoL->settings.sizeY * sizeof(char*));
    for(i = 0; i < GoL->settings.sizeY; i++) {
        GoL->currentIteration[i] = malloc(GoL->settings.sizeX * sizeof(char));
    }

    GoL->nextIteration = malloc(GoL->settings.sizeY * sizeof(char*));
    for(i = 0; i < GoL->settings.sizeY; i++) {
        GoL->nextIteration[i] = malloc(GoL->settings.sizeX * sizeof(char));
    }

    for(o = 0; o < GoL->settings.sizeY; o++) {
        for(i = 0; i < GoL->settings.sizeX; i++) {
            GoL->currentIteration[o][i] = rand() % 2 ? GoL->settings.aliveCellChar : GoL->settings.deadCellChar;
        }
    }
}
