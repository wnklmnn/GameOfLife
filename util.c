#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#include "config.h"

int readStartArguments(int argc, char** argv, GameOfLife *GoL){

	int i;
	// Default Settings
    GoL->settings.deadCellChar = ' ';
    GoL->settings.aliveCellChar = 'X';
    GoL->settings.sizeX = 20;
    GoL->settings.sizeY = 20;
    GoL->settings.zufallsStart = 'y';
    GoL->settings.stepByStep = 'n';
    GoL->settings.edgeBehavior = 0;
    GoL->settings.importField = 0;
    GoL->settings.exportField = 0;
    GoL->interationPerSecond = 0;
    GoL->interationCount = 0;
    GoL->settings.rule_aliveNumber = 23;
    GoL->settings.rule_birthNumber = 3;
    GoL->iteration = 0;

    /* Argument Parsing */
    for(i=1;i<argc; i++){
        /* Arguments without value */
        if(strcmp(argv[i], "-v") == 0) {
            printf("GameOfLife v%s\nCompiled at %s %s\n with GCC %s on %s\n", VERSION, __DATE__, __TIME__, __VERSION__, OS);
            return 1;
        } else if(strcmp(argv[i], "-random") == 0) {
            GoL->settings.zufallsStart = 'y';
            GoL->settings.stepByStep = 'n';
        } else if(strcmp(argv[i], "-step") == 0) {
            GoL->settings.zufallsStart = 'n';
            GoL->settings.stepByStep = 'y';
        } else if(strcmp(argv[i], "-import") == 0) {
            GoL->settings.importField = 1;
        } else if(strcmp(argv[i], "-export") == 0) {
            GoL->settings.exportField = 1;
        } else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
            printf("Usage: GameOfLife \n");
            printf("-v\t\tShows the programm version\n");
            printf("-ac\t\tDefines the charactor a alive cell\n");
            printf("-dc\t\tDefines the charactor a dead cell\n");
            printf("-random\t\tStart with a random start\n");
            printf("-x\t\tDefines the width of the field\n");
            printf("-y\t\tDefines the height of the field\n");
            printf("-step\t\tRun the game by pressing the enter key\n");
            printf("-eb\t\tChange the behavior how the Edge of the Array is handeld\n");
            printf("-import\t\tImport the exported field\n");
            printf("-export\t\tExport the current game to file \"export.txt\" (required step mode)\n");

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
                } else if(strcmp(argName, "-ra") == 0) {
                    GoL->settings.rule_aliveNumber = atoi(argValue);
                } else if(strcmp(argName, "-rb") == 0) {
                    GoL->settings.rule_birthNumber = atoi(argValue);
                } else if(strcmp(argName, "-x") == 0) {
                    int val = atoi(argValue);
                    if(val > 0 ) {
                        GoL->settings.sizeX = val;
                    }
                } else if(strcmp(argName, "-y") == 0) {
                    int val = atoi(argValue);
                    if(val > 0 ) {
                        GoL->settings.sizeY = val;
                    }
                } else if(strcmp(argName, "-eb") == 0) {
                    GoL->settings.edgeBehavior = atoi(argValue);
                    if (GoL->settings.edgeBehavior<0 || GoL->settings.edgeBehavior>2){
                        GoL->settings.edgeBehavior = 0;
                    }
                }
            }
        }
    }

    return 0;
}

void exportSpielFeld(GameOfLife *GoL) {
    FILE *f = fopen("export.txt", "w");
    int i, o;
    /*
        1 => Random/Step (0 = Random, 1= Step)
        2 => Alive Char
        3 => Dead Char
        4 => EdgeBehavior
        5 => Alivenumber
        6 => Birthnumber
        7 => Interationsnummer
    */

    fprintf(f, "%i;%c;%c;%i;%i;%i;%i\n", ((GoL->settings.zufallsStart == (int)"y") ? 0 : 1), GoL->settings.aliveCellChar, GoL->settings.deadCellChar, GoL->settings.edgeBehavior, GoL->settings.rule_aliveNumber, GoL->settings.rule_birthNumber, GoL->iteration);
    /* Spielfeld Größe */
    fprintf(f, "%i;%i\n", GoL->settings.sizeX, GoL->settings.sizeY);
    for(o=0;o<GoL->settings.sizeY;o++){
	    for(i=0;i<GoL->settings.sizeX;i++){
            fprintf(f, "%i;", (GoL->currentIteration[o][i] == GoL->settings.aliveCellChar) ? 1 : 0);
	    }
	    if(o != GoL->settings.sizeY - 1) {
            fprintf(f, "\n");
	    }
	}
	fclose(f);
}

int importSpielFeld(GameOfLife *GoL) {
    int i;
    if(access("export.txt", F_OK) != -1) {
        FILE *f = fopen("export.txt", "rb");
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);
        char *string = malloc(fsize + 1);
        fread(string, fsize, 1, f);
        fclose(f);

        char *line = strtok(string, ";");
        int xSize = atoi(line);
        int ySize = 0;

        line = strtok(NULL, "\n");
        ySize = atoi(line);

        GoL->currentIteration = malloc(ySize * sizeof(char*));
        for(i=0;i<ySize;i++){
            GoL->currentIteration[i] = malloc(xSize * sizeof(char));
        }

        GoL->nextIteration = malloc(ySize * sizeof(char*));
        for(i=0;i<ySize;i++){
            GoL->nextIteration[i] = malloc(xSize * sizeof(char));
        }

        int currentX = 0;
        int currentY = 0;
        line = strtok(NULL, "\n");

        while(line != NULL) {
            printf("Row: %s\n", line);

            char *column = strtok(NULL, ";");

            for(currentX=0;i<xSize;currentX++){
                printf("X: %i, Y: %i => %i\n", currentX, currentY, atoi(column));

                column = strtok(NULL, ";");
            }

            line = strtok(NULL, "\n");
            currentY++;
        }

        return 0;

    } else {
        return -1;
    }
}
