#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "hrac.h"

#ifndef SNAKE_HRACIE_POLE_H
#define SNAKE_HRACIE_POLE_H

#define BOARD_SIZE_X 800
#define BOARD_SIZE_Y 800

static enum CellType {
    PRAZDNE,
    HAD1,
    HAD2,
    JEDLO
};


//static int quit = 0;
void initGame(HRAC* hrac1,HRAC* hrac2);
void drawGameBoard(HRAC* hrac1, HRAC* hrac2);
void gameLoop(HRAC* hrac1,HRAC* hrac2);
void renderCell(HRAC* hrac);

static enum CellType gameBoard[BOARD_SIZE_X][BOARD_SIZE_Y];

#endif //SNAKE_HRACIE_POLE_H