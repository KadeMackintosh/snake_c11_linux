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

void initGame(HRAC* hrac);
void gameLoop(HRAC* hrac);

static enum CellType gameBoard[BOARD_SIZE_X][BOARD_SIZE_Y];

#endif //SNAKE_HRACIE_POLE_H