#include<stdio.h>
#include<stdlib.h>

struct GameOfLifeSettings{
    char deadCellChar;
    char aliveCellChar;
    int randomBeginning;
    int sizeX;
    int sizeY;
    char stepByStep;
    char zufallsStart;
};

void Ausgabe2DArray(char** Array, int sizeX, int sizeY);
char** ErstelleSpielfeld( int sizeY, int sizeX, char initialValue);

int main(int argc, char** argv){
    int i;
    char input[5];
    char **currentIteration;
    char **nextIteration;
    struct GameOfLifeSettings GoL;
    GoL.deadCellChar = '0';

    //Ausgabe der Commandline Parameter
    printf("Kommandozeilenparameter:\n");
    for(i=0;i<argc; i++){
            printf("%i: %s\n", i, argv[i]);
    }


    // einlesen der Einstellungen
    //Hoehe
    printf("Wie hoch soll das Spielfeld sein?: ");
	do{
	    fgets(input, sizeof(input), stdin);
	}while(sscanf(input, "%i", &GoL.sizeY)!=1);
	
	//Breite
    printf("Wie breit soll das Spielfeld sein?: ");
	do{
	    fgets(input, sizeof(input), stdin);
	}while(sscanf(input, "%i", &GoL.sizeX)!=1);
	
	//Zufallsstart
	printf("Soll das Feld zufaellig erstellt werden?[y|n]:");
	do{
	    fgets(input, sizeof(input), stdin);
	}while(sscanf(input, " %c", &GoL.zufallsStart)!=1 || (GoL.zufallsStart!='y'&& GoL.zufallsStart!='n'));
	
	//schritt-fuer-Schritt-betrieb
	printf("Wollen sie die Iterationen einzeln durchgehen?[y|n]:");
	do{
	    fgets(input, sizeof(input), stdin);
	}while(sscanf(input, " %c", &GoL.stepByStep)!=1 || (GoL.stepByStep!='y'&& GoL.stepByStep!='n'));
	
	
	//Initialize Array
	currentIteration = ErstelleSpielfeld( GoL.sizeY, GoL.sizeX, GoL.deadCellChar);
	nextIteration = ErstelleSpielfeld( GoL.sizeY, GoL.sizeX, GoL.deadCellChar);
	
	
	
	
	
    Ausgabe2DArray(currentIteration, GoL.sizeX, GoL.sizeY);
    Ausgabe2DArray(nextIteration, GoL.sizeX, GoL.sizeY);
    return 0;
}

void Ausgabe2DArray(char** Array, int sizeX, int sizeY){
    int o, i;
    printf("Ausgabe Array %i*%i\n", sizeY, sizeX);
	for (o=0; o< sizeY;o++){
	    for(i=0;i<sizeX;i++){
	        printf("%c ", Array[o][i]);
	    }
	    printf("\n");
    }
}

char** ErstelleSpielfeld( int sizeY, int sizeX, char initialValue){
    int i, o;
    char **feld;
    feld = malloc(sizeY * sizeof(char*));
	for(i=0;i<sizeY;i++){
	    feld[i] = malloc(sizeX * sizeof(char));
	}
	for (o=0; o< sizeY;o++){
	    for(i=0;i<sizeX;i++){
	        feld[o][i] = initialValue;
	    }
    }
	return feld;
}
