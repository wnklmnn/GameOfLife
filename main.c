#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "util.h"
#include "drawing.h"
#include "gol.h"

#ifdef __linux__
#include <sys/time.h>
#endif

int main(int argc, char** argv)
{
    /*
    Zeit für FPS Begrenzung speichern
    */

#ifdef WIN32
    //WindowsImplementation
    double tmpTime1, tmpTime2;
#else
    struct timeval tv;
    double tmpTime1, tmpTime2;
#endif
    double timePerFrame;
    int keepRunning = 0;

    srand(time(NULL));
    GameOfLife GoL;
    GoL.lastTimestamp = (unsigned)time(NULL);

    /*
      Argumente beim starten des Programmes lesen, auswerten und ins "GoL" schreiben
    */
    if(readStartArguments(argc, argv, &GoL) == 1) {
        return 0;
    }
    timePerFrame = 1000 / GoL.settings.fps;

    /*
        Ncurses Sitzung starten
    */
    initStartScreen();
    WINDOW* headWin = subwin(stdscr, 1, COLS, 0, 0);
    WINDOW* subWin = subwin(stdscr, LINES - 1, COLS, 1, 0);

    if(GoL.settings.consoleColor != 0) {
        wbkgd(subWin, COLOR_PAIR(GoL.settings.consoleColor));
    }

    updateHeadWin(headWin, GoL);

    if(strcmp(GoL.settings.importFile, "")) {
        if(importSpielFeld(&GoL) == -1) {
            wmessage(subWin, "Import fehlgeschlagen.\nTippe eine beliebige Taste um zufaelliges Spielfeld zu erstellen.\n");
            ErstelleSpielfeld(&GoL);
            ErstellePastIterations(&GoL);
        }
    } else if(strcmp(GoL.settings.exportFile, "") && GoL.settings.stepByStep == 'n') {
        wmessage(subWin, "Export abgebrochen: Export funktioniert nur beim Step by Step Mode\nTippe eine beliebige Taste um zufälliges Spielfeld zu erstellen.\n");
        ErstelleSpielfeld(&GoL);
        ErstellePastIterations(&GoL);
    } else {
        ErstelleSpielfeld(&GoL);
        ErstellePastIterations(&GoL);
    }


    while(1) {
#ifdef WIN32
        //WindowsImplementation
        tmpTime1 = GetTickCount();
#else
        //Linux Implementation
        gettimeofday(&tv, NULL);
        tmpTime1 = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
#endif
        CalcIteration(&GoL);
        AusgabeSpielfeld(GoL, subWin);

        /*
            Stabilen Zustand erkennen
        */
        if(keepRunning == 0 && UeberpruefeSpielfeldAufLoop(GoL) == 1) {
            wmessage(subWin, "Es wurde ein stabiler Zustand festgestellt.\nDurecken Sie eine beliebige Taste um die Ausfuehrung fortzusetzen.\n");
            keepRunning = 1;
        }
        updateHeadWin(headWin, GoL);
        if (GoL.settings.stepByStep == 'y') {
            if(strcmp(GoL.settings.exportFile, "")) {
                exportSpielFeld(&GoL);
            }
            getchar();

            // Buffer der Console leeren
            fseek(stdin, 0, SEEK_END);
        } else {
#ifdef WIN32
            //WindowsImplementation
            do {
                usleep(1);
                tmpTime2 = GetTickCount();
            } while(tmpTime2 < tmpTime1 + timePerFrame);
#else
            //Linux Implementation
            do {
                usleep(1);
                gettimeofday(&tv, NULL);
                tmpTime2 = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000 ;
            } while(tmpTime2 < tmpTime1 + timePerFrame);
#endif
        }
    }

    return 0;
}
