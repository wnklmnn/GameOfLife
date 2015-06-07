#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "util.h"
#include "config.h"
#include "gol.h"

int readStartArguments(int argc, char** argv, GameOfLife *GoL){

	int i;
	GoL->interationPerSecond = 0;
	GoL->iteration = 0;
    GoL->interationCount = 0;

	// Default Settings
    GoL->settings.deadCellChar = ' ';
    GoL->settings.aliveCellChar = 'X';
    GoL->settings.sizeX = 20;
    GoL->settings.sizeY = 20;
    GoL->settings.zufallsStart = 'y';
    GoL->settings.stepByStep = 'n';
    GoL->settings.edgeBehavior = 0;
    GoL->settings.rule_aliveNumber = "23";
    GoL->settings.rule_birthNumber = "3";
    GoL->settings.numberOfPastIterations = 20;
    GoL->settings.fps = 10;
    GoL->settings.importFile = "";
    GoL->settings.exportFile = "";
    GoL->settings.consoleColor = 1;

    /* Argument Parsing */
    for(i=1;i<argc; i++){
        /* Arguments without value */
        if(strcmp(argv[i], "-v") == 0) {
            printf("GameOfLife v%s\nCompiled at %s %s\nwith GCC %s on %s\n", VERSION, __DATE__, __TIME__, __VERSION__, OS);
            return 1;
        } else if(strcmp(argv[i], "-random") == 0) {
            GoL->settings.zufallsStart = 'y';
        } else if(strcmp(argv[i], "-export") == 0) {
            GoL->settings.exportFile = "export.txt";
        } else if(strcmp(argv[i], "-import") == 0) {
            GoL->settings.importFile = "import.txt";
        } else if(strcmp(argv[i], "-step") == 0) {
            GoL->settings.stepByStep = 'y';
        } else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "-help") == 0) {
            printf("Usage: GameOfLife \n");
            printf("-v\t\tShows the programm version\n");
            printf("-ac\t\tDefines the character of an alive cell\n");
            printf("-dc\t\tDefines the character of a dead cell\n");
            printf("-random\t\tStart with a random start\n");
            printf("-x\t\tDefines the width of the field\n");
            printf("-y\t\tDefines the height of the field\n");
            printf("-step\t\tRun the game by pressing the enter key\n");
            printf("-eb\t\tChange the behavior how the Edge of the Array is handeld\n");
            printf("-import\t\tImport the field from file\n");
            printf("-export\t\tExport the current game to file (required step mode, default filename \"export.txt\")\n");
            printf("-ra\t\tHow many neighbour cells need to be alive to keep the keep the cell alive in the next iteration.\n");
            printf("-rb\t\tHow many neighbour cells need to be alive so that a currently dead cell is gettin alive again.\n");
            printf("-ld\t\tThe number of past iterations to be used for the loop detection\n");
            printf("-color\t\tChoose console color (see -colors for all available colors)\n");

            return 1;
        } else if(strcmp(argv[i], "-colors") == 0) {
            printf("Available Colors:\n");
            printf("1\t=>\tBlack/White\n");
            printf("2\t=>\tBlack/Green\n");
            printf("3\t=>\tWhite/Black\n");
            printf("4\t=>\tRed/White\n");
            printf("5\t=>\tGreen/White\n");
            printf("6\t=>\tBlue/White\n");

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
                    GoL->settings.rule_aliveNumber = argValue;
                } else if(strcmp(argName, "-rb") == 0) {
                    GoL->settings.rule_birthNumber = argValue;
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
                } else if(strcmp(argName, "-fps") == 0) {
                    GoL->settings.fps = atoi(argValue);
                    if (GoL->settings.fps<0 ){
                        GoL->settings.fps = 1;
                    }
                } else if(strcmp(argName, "-ld") == 0) {
                    GoL->settings.numberOfPastIterations = atoi(argValue);
                    if (GoL->settings.numberOfPastIterations < 1 ){
                        GoL->settings.numberOfPastIterations = 20;
                    }
                } else if(strcmp(argName, "-color") == 0) {
                    int val = atoi(argValue);
                    if(val > 0 && val < 7) {
                        GoL->settings.consoleColor = val;
                    }
                } else if(strcmp(argName, "-import") == 0) {
                    GoL->settings.importFile = argValue;
                } else if(strcmp(argName, "-export") == 0) {
                    GoL->settings.exportFile = argValue;
                }
            }
        }
    }

    return 0;
}

void exportSpielFeld(GameOfLife *GoL) {
    int i, o;
    FILE *f = fopen(GoL->settings.exportFile, "w");
    fprintf(f, "%i;%i;%i;", GoL->settings.sizeX, GoL->settings.sizeY, GoL->iteration);
    for(o=0;o<GoL->settings.sizeY;o++){
	    for(i=0;i<GoL->settings.sizeX;i++){
            fprintf(f, "%i", (GoL->currentIteration[o][i] == GoL->settings.aliveCellChar) ? 1 : 0);
	    }
	}
	fclose(f);
}

int importSpielFeld(GameOfLife *GoL) {
    int o, i;
    int intChar;
    long int startPosOfsizeX;
    long int endPosOfsizeX;
    long int startPosOfsizeY;
    long int endPosOfsizeY;
    char* tmpChar;
    FILE *f = fopen(GoL->settings.importFile, "r");
    //Find SizeX in file
    startPosOfsizeX = 0;
    do{
        intChar = fgetc(f);
    }while(intChar != ';');
    endPosOfsizeX = ftell(f);
    tmpChar = malloc(((endPosOfsizeX-startPosOfsizeX)+1)*sizeof(char));
    fseek(f, startPosOfsizeX, SEEK_SET);
    fread(tmpChar, sizeof(char), (endPosOfsizeX-startPosOfsizeX), f);
    GoL->settings.sizeX = atoi(tmpChar);
    free(tmpChar);

    //Find SizeY in file
    startPosOfsizeY = endPosOfsizeX;
    do{
        intChar = fgetc(f);
    }while(intChar != ';');
    endPosOfsizeY = ftell(f);

    tmpChar = malloc(((endPosOfsizeY-startPosOfsizeY)+1)*sizeof(char));
    fseek(f, startPosOfsizeY, SEEK_SET);
    fread(tmpChar, sizeof(char), (endPosOfsizeY-startPosOfsizeY), f);
    GoL->settings.sizeY = atoi(tmpChar);

    free(tmpChar);

    // Read Interation
    startPosOfsizeY = endPosOfsizeX;
    do{
        intChar = fgetc(f);
    }while(intChar != ';');
    endPosOfsizeY = ftell(f);

    tmpChar = malloc(((endPosOfsizeY-startPosOfsizeY)+1)*sizeof(char));
    fseek(f, startPosOfsizeY, SEEK_SET);
    fread(tmpChar, sizeof(char), (endPosOfsizeY-startPosOfsizeY), f);
    GoL->iteration = atoi(tmpChar);

    free(tmpChar);

    ErstelleSpielfeld(GoL);
    ErstellePastIterations(GoL);


    fseek(f, endPosOfsizeY, SEEK_SET);
    for(o=0; o<GoL->settings.sizeY; o++){
        for(i=0; i<GoL->settings.sizeX; i++){
            intChar = fgetc(f);
            if(intChar == '1'){
                GoL->currentIteration[o][i] = GoL->settings.aliveCellChar;
            }else if(intChar == '0'){
                GoL->currentIteration[o][i] = GoL->settings.deadCellChar;
            }
        }
    }

    fclose(f);
    return 0;
}
