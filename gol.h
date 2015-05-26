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
    char *rule_birthNumber;
    char *rule_aliveNumber;
} GameOfLifeSettings;
typedef struct {
    unsigned int iteration;
    GameOfLifeSettings settings;
    char **currentIteration;
    char **nextIteration;
    unsigned int lastTimestamp;
    int interationPerSecond;
    int interationCount;
} GameOfLife;


void ErstelleSpielfeld(GameOfLife *GoL);
void CalcIteration(GameOfLife *GoL);


#endif // GOL_H_INCLUDED
