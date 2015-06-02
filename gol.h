#ifndef GOL_H_INCLUDED
#define GOL_H_INCLUDED

typedef struct{
    char deadCellChar;
    char aliveCellChar;
    int randomBeginning;
    int sizeX;
    int sizeY;
    int edgeBehavior; //0=deadCell, 1=aliveCell, 2=Torus
    char stepByStep;
    char zufallsStart;
    int importField;
    int exportField;
    char* rule_birthNumber;
    char* rule_aliveNumber;
    int fps;
    int numberOfPastIterations;
} GameOfLifeSettings;
typedef struct {
    unsigned int iteration;
    GameOfLifeSettings settings;
    char **currentIteration;
    char **nextIteration;
    unsigned int lastTimestamp;
    int interationPerSecond;
    int interationCount;
    char ***pastIterations;
} GameOfLife;


int  UeberpruefeSpielfeldAufLoop(GameOfLife GoL);
void ErstellePastIterations(GameOfLife *GoL);
void ErstelleSpielfeld(GameOfLife *GoL);
void CalcIteration(GameOfLife *GoL);


#endif // GOL_H_INCLUDED
