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


void initGame(enum CellType (*gameBoard)[BOARD_SIZE_X][BOARD_SIZE_Y]);
void gameLoop(HRAC* hrac, SDL_Event *event, enum CellType (*gameBoard)[BOARD_SIZE_X][BOARD_SIZE_Y]);

static enum CellType gameBoard[BOARD_SIZE_X][BOARD_SIZE_Y];

#endif //SNAKE_HRACIE_POLE_H