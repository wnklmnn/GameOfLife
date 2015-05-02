/* Structs */
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
int readStartArguments(int argc, char** argv, struct GameOfLife *GoL);