#include<stdio.h>
#include<stdlib.h>

struct GameOfLifeSettings{
    char deadCellChar;
    char aliveCellChar;
    int randomBeginning;
    int sizeX;
    int sizeY;
    int edgeBehavior; //0=deadCell, 1=aliveCell, 2=Torus
    char stepByStep;
    char zufallsStart;
};
struct GameOfLife{
    unsigned int iteration;
    struct GameOfLifeSettings settings;
    char **currentIteration;
    char **nextIteration;
};

void Ausgabe2DArray(char** Array, int sizeX, int sizeY);
char** ErstelleSpielfeld( int sizeY, int sizeX, char initialValue);
void CalcIteration(struct GameOfLife GoL);

int main(int argc, char** argv){
    int i;
    struct GameOfLife GoL;
    GoL.settings.deadCellChar = ' ';
    GoL.settings.aliveCellChar = 'X';
    GoL.settings.sizeX = 20;
    GoL.settings.sizeY = 20;
    GoL.settings.zufallsStart = 'y';
    GoL.settings.stepByStep = 'n';
    GoL.settings.edgeBehavior = 0;

    //Ausgabe der Commandline Parameter
    printf("Kommandozeilenparameter:\n");
    for(i=0;i<argc; i++){
            printf("%i: %s\n", i, argv[i]);
    }

    /*
    // einlesen der Einstellungen
    char input[5];
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
	*/
	
	//Initialize Array
	GoL.currentIteration = ErstelleSpielfeld( GoL.settings.sizeY, GoL.settings.sizeX, GoL.settings.aliveCellChar);
	GoL.nextIteration = ErstelleSpielfeld( GoL.settings.sizeY, GoL.settings.sizeX, GoL.settings.deadCellChar);
	
	
    while(1==1){
        CalcIteration(GoL);
        Ausgabe2DArray(GoL.currentIteration, GoL.settings.sizeX, GoL.settings.sizeY);
        getchar();
    }	
	
	
    Ausgabe2DArray(GoL.currentIteration, GoL.settings.sizeX, GoL.settings.sizeY);
    Ausgabe2DArray(GoL.nextIteration, GoL.settings.sizeX, GoL.settings.sizeY);
    return 0;
}

void CalcIteration(struct GameOfLife GoL){
    int o, i;
    int aliveNeightbors;
    char **tmpIteration;
    for (o=0; o<GoL.settings.sizeY; o++){
        for(i=0; i<GoL.settings.sizeX; i++){
        //zaehlen der Lebenden Nachbarn je nach einstellung
            if (GoL.settings.edgeBehavior==0){ // Ausßerhalb sind Tote Zellen
                aliveNeightbors = 0;
                //OL
                aliveNeightbors += (GoL.currentIteration[o-1][i-1]==GoL.settings.aliveCellChar && o>0 && i > 0) ? 1 : 0;
                //O
                aliveNeightbors += (GoL.currentIteration[o-1][i]==GoL.settings.aliveCellChar && o>0) ? 1 : 0;
                //OR
                aliveNeightbors += (GoL.currentIteration[o-1][i+1]==GoL.settings.aliveCellChar && o>0 && i < GoL.settings.sizeX) ? 1 : 0;
                //L
                aliveNeightbors += (GoL.currentIteration[o][i-1]==GoL.settings.aliveCellChar && i > 0) ? 1 : 0;
                //R
                aliveNeightbors += (GoL.currentIteration[o-1][i-1]==GoL.settings.aliveCellChar && i < GoL.settings.sizeX) ? 1 : 0;
                //UL
                aliveNeightbors += (GoL.currentIteration[o+1][i-1]==GoL.settings.aliveCellChar && o<GoL.settings.sizeY && i > 0) ? 1 : 0;
                //U
                aliveNeightbors += (GoL.currentIteration[o+1][i]==GoL.settings.aliveCellChar && o<GoL.settings.sizeY) ? 1 : 0;
                //UR
                aliveNeightbors += (GoL.currentIteration[o+1][i+1]==GoL.settings.aliveCellChar && o<GoL.settings.sizeY && i < GoL.settings.sizeX) ? 1 : 0;
            }else if (GoL.settings.edgeBehavior == 1){ //Ausßerhalb sind Lebende Zellen
                
            }else if (GoL.settings.edgeBehavior == 2){ // Das spielfeld ist Kugelförmig. 
                
            }
        //Lebt oder stirbt die Zelle in der nächsten Generation?
        //Hier könnte man noch verschiedene Regeln einbauen?
            if(GoL.currentIteration[o][i] == GoL.settings.aliveCellChar && aliveNeightbors < 2){
                GoL.nextIteration[o][i] = GoL.settings.deadCellChar;
            }
            if (GoL.currentIteration[o][i] == GoL.settings.deadCellChar && aliveNeightbors == 3){
                GoL.nextIteration[o][i] = GoL.settings.aliveCellChar;
            }
            if (GoL.currentIteration[o][i] == GoL.settings.aliveCellChar && (aliveNeightbors == 2 || aliveNeightbors == 3)){
                GoL.nextIteration[o][i] = GoL.settings.aliveCellChar;
            }
            if(GoL.currentIteration[o][i] == GoL.settings.aliveCellChar && aliveNeightbors > 3){
                GoL.nextIteration[o][i] = GoL.settings.deadCellChar;
            }
            
        }
    }
    tmpIteration = GoL.currentIteration;
    GoL.currentIteration = GoL.nextIteration;
    GoL.nextIteration = tmpIteration;
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
