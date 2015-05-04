/* Structs */
typedef struct{
    char deadCellChar;
    char aliveCellChar;
    int randomBeginning;
    int sizeX;
    int sizeY;
    int edgeBehavior; //0=deadCell, 1=aliveCell, 2=Torus
    char stepByStep;
    char zufallsStart;
} GameOfLifeSettings;
typedef struct {
    unsigned int iteration;
    GameOfLifeSettings settings;
    char **currentIteration;
    char **nextIteration;
} GameOfLife;
int readStartArguments(int argc, char** argv, GameOfLife *GoL);
