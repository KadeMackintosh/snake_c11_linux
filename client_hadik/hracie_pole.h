#include "hrac.h"

#ifndef SNAKE_HRACIE_POLE_H
#define SNAKE_HRACIE_POLE_H

#define BOARD_SIZE_X 800
#define BOARD_SIZE_Y 800

static enum CellType {
    PRAZDNE,
    HAD,
    JEDLO
};

// static enum Movement {
//     UP = {},
//     DOWN,
//     LEFT,
//     RIGHT
// };


void initGame();
void gameLoop(HRAC* hrac1,HRAC* hrac2, SDL_Event *event);
void renderCell(HRAC* hrac);

static enum CellType gameBoard[BOARD_SIZE_X][BOARD_SIZE_Y];

#endif //SNAKE_HRACIE_POLE_H