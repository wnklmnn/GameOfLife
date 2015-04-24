#include<stdio.h>
#include<stdlib.h>


void Ausgabe2DArray(char** Array, int sizeX, int sizeY);
char** ErstelleSpielfeld( int sizeY, int sizeX, char initialValue);

int main(){
    int sizeX=0;
    int sizeY=0;
    char zufallsStart;
    char stepbystep;
    char input[5];
    char **currentIteration;
    char **nextIteration;
    // einlesen der Einstellungen
    //Hoehe
    printf("Wie hoch soll das Spielfeld sein?: ");
	do{
	    fgets(input, sizeof(input), stdin);
	}while(sscanf(input, "%i", &sizeY)!=1);
	
	//Breite
    printf("Wie breit soll das Spielfeld sein?: ");
	do{
	    fgets(input, sizeof(input), stdin);
	}while(sscanf(input, "%i", &sizeX)!=1);
	
	//Zufallsstart
	printf("Soll das Feld zufaellig erstellt werden?[y|n]:");
	do{
	    fgets(input, sizeof(input), stdin);
	}while(sscanf(input, " %c", &zufallsStart)!=1 || (zufallsStart!='y'&& zufallsStart!='n'));
	
	//schritt-fuer-Schritt-betrieb
	printf("Wollen sie die Iterationen einzeln durchgehen?[y|n]:");
	do{
	    fgets(input, sizeof(input), stdin);
	}while(sscanf(input, " %c", &stepbystep)!=1 || (stepbystep!='y'&& stepbystep!='n'));
	
	
	//Initialize Array
	currentIteration = ErstelleSpielfeld( sizeY, sizeX, '0');
	nextIteration = ErstelleSpielfeld( sizeY, sizeX, '0');
	
	
	
	
	
    Ausgabe2DArray(currentIteration, sizeX, sizeY);
    Ausgabe2DArray(nextIteration, sizeX, sizeY);
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
	        feld[o][i] = '0';
	    }
    }
	return feld;
}
