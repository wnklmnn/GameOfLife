#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "util.h"
#include "config.h"

int readStartArguments(int argc, char** argv, struct GameOfLife *GoL){
	
	int i;
	// Default Settings
    GoL->settings.deadCellChar = ' ';
    GoL->settings.aliveCellChar = 'X';
    GoL->settings.sizeX = 20;
    GoL->settings.sizeY = 20;
    GoL->settings.zufallsStart = 'y';
    GoL->settings.stepByStep = 'n';
    GoL->settings.edgeBehavior = 0;
    GoL->iteration = 0;

    /* Argument Parsing */
    for(i=1;i<argc; i++){
        /* Arguments without value */
        if(strcmp(argv[i], "-v") == 0) {
            printf("GameOfLife v%s\n", VERSION);
            return 1;
        } else if(strcmp(argv[i], "-random") == 0) {
            GoL->settings.zufallsStart = 'y';
            GoL->settings.stepByStep = 'n';
        } else if(strcmp(argv[i], "-step") == 0) {
            GoL->settings.zufallsStart = 'n';
            GoL->settings.stepByStep = 'y';
        } else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
            printf("Usage: GameOfLife \n");
            printf("-v\t\tShows the programm version\n");
            printf("-ac\t\tDefines the charactor a alive cell\n");
            printf("-dc\t\tDefines the charactor a dead cell\n");
            printf("-random\t\tStart with a random start\n");
            printf("-x\t\tDefines the width of the field\n");
            printf("-y\t\tDefines the height of the field\n");
            printf("-step\t\tRun the game by pressing the enter key\n");

            return 1;
        } else {
            /* Arguments with value */
            char *argName = strtok(argv[i], "=");
            if(argName != NULL) {
                char *argValue = strtok(NULL, "=");
                if(strcmp(argName, "-ac") == 0) {
                    GoL->settings.aliveCellChar = *argValue;
                } else if(strcmp(argName, "-dc") == 0) {
                    GoL->settings.deadCellChar = *argValue;
                } else if(strcmp(argName, "-x") == 0) {
                    GoL->settings.sizeX = atoi(argValue);
                } else if(strcmp(argName, "-y") == 0) {
                    GoL->settings.sizeY = atoi(argValue);
                }
            }
        }
    }

    return 0;
}
