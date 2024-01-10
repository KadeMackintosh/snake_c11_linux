//
// Created by kad3m on 12/27/2023.
//

#include <stdio.h>
#include "hrac.h"

HRAC* vytvorHraca(const char* meno, int dlzka) {
    HRAC* novyHrac = malloc(sizeof(HRAC));
    if (novyHrac == NULL) {
        
        //fprintf(stderr, "Chyba pri vytvarani hraca");
        return NULL;
    }

    novyHrac->meno = malloc(strlen(meno) + 1); // +1 pre ten \0 terminator
    if (novyHrac->meno == NULL) {
        free(novyHrac);
        //fprintf(stderr, "Chyba pri priradeni mena hracovi");
        return NULL;
    }

    novyHrac->HADIK = malloc(sizeof(struct hadik));

    if (novyHrac->HADIK != NULL)
    {
        novyHrac->HADIK->snakeBodyX = calloc(MAX_SNAKE_LENGTH, sizeof(int));
        novyHrac->HADIK->snakeBodyY = calloc(MAX_SNAKE_LENGTH, sizeof(int));
        novyHrac->HADIK->dlzka = dlzka;
    }

    strcpy(novyHrac->meno, meno);

    return novyHrac;
}

void destroyHrac(HRAC* hrac) {
    free(hrac->HADIK->snakeBodyX);
    free(hrac->HADIK->snakeBodyY);
    free(hrac->HADIK);
    free(hrac->meno);
    free(hrac);
}