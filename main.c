#include <stdio.h>
#include <SDL.h>
#include <pthread.h>
// #include "menu.h"
#include "hracie_pole.h"

#include "hrac.h"

struct arguments{
	HRAC* hrac;
	SDL_Event* event;
};

// Otazky:
// je architektura ok?
// sú premenné izolované vo svojich vláknach? Ako to funguje?
// client/server?
// SDL showMessageBox nefunguje na Linux?

void* playerThread(void* arg) {

	struct arguments* localArgs = (struct arguments*)arg;
	gameLoop(localArgs->hrac, localArgs->event);
	// cleanupSDL();
	return NULL;
}

int main() {
	pthread_t thread1, thread2; // You can extend this for more players

	HRAC* hrac1 = vytvorHraca("Kade", 1);
	hrac1->HADIK->x = 1;
	hrac1->HADIK->y = 1;
	hrac1->HADIK->snakeDirectionX = 1; // Initial direction (right)
	hrac1->HADIK->snakeDirectionY = 0;
    
	// HRAC* hrac2 = vytvorHraca("Marek", 1);
	// hrac2->HADIK->x = 1;
	// hrac2->HADIK->y = 3;
	// hrac2->HADIK->snakeDirectionX = 1;
	// hrac2->HADIK->snakeDirectionY = 0;
	

	SDL_Event event1;
	struct arguments* hrac1Args = malloc(sizeof(struct arguments));
	hrac1Args->event = &event1;
	hrac1Args->hrac = hrac1;

	// SDL_Event event2;
	// struct arguments* hrac2Args = malloc(sizeof(struct arguments));
	// hrac2Args->event = &event2;
	// hrac2Args->hrac = hrac2;

	initGame();
	initSnake(hrac1);
	// initSnake(hrac2);
	randomFood();
	drawGameBoard();
	pthread_create(&thread1, NULL, playerThread, hrac1Args);
	// gameLoop(hrac2);
	// pthread_create(&thread2, NULL, playerThread, hrac2Args);

	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);

	while (1)
	{
		/* code */
	}
	// initMenu();

	// menuLoop(hrac1);
	// cleanUpMenu();

	return 0;
}
