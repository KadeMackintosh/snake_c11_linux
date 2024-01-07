//
// Created by kad3m on 12/27/2023.
//

#ifndef SNAKE_HRAC_H
#define SNAKE_HRAC_H

#include <malloc.h>
#include <string.h>

#define MAX_SNAKE_LENGTH 100

typedef struct hrac {
    char* meno;
    struct hadik {
        int dlzka;
        int x;
        int y;
        int* snakeBodyX;
        int* snakeBodyY;
        int snakeDirectionX;
        int snakeDirectionY;
    } *HADIK;
} HRAC;

HRAC* vytvorHraca(const char* meno, int dlzka);
void destroyHrac(HRAC* hrac);

#endif //SNAKE_HRAC_H
