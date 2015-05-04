#ifdef WIN32
#include "vendor/pdcurses/curses.h"
#include <windows.h>
#else
#include <curses.h>
#endif

#ifndef DRAWING_H_INCLUDED
#define DRAWING_H_INCLUDED
void initStartScreen();
void updateHeadWin(WINDOW *headWin, GameOfLife GoL);
void AusgabeSpielfeld(GameOfLife GoL, WINDOW* subWin);


#endif // DRAWING_H_INCLUDED
