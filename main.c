#include <stdio.h>
#include <SDL.h>
#include <pthread.h>
// #include "menu.h"
#include "hracie_pole.h"

#include "hrac.h"

// pthread_mutex_t mutex;
// pthread_mutex_t mutex2; 
struct arguments
{
	HRAC *hrac;
	SDL_Event *event;
	SDL_bool arrow_control;
};

// 1 vlakno vykreslovacie (updateSnakePosition)
// 2 vlakna pre hracov (ktori odovzdavaju info do hlavneho vlakna)
//

// Otazky:
// je architektura ok?
// sú premenné izolované vo svojich vláknach? Ako to funguje?
// client/server?
// SDL showMessageBox nefunguje na Linux?

HRAC *hrac1;
HRAC *hrac2;

void *playerThreadFunc(void *arg)
{
	struct arguments *localArgs = (struct arguments *)arg;
	//pthread_mutex_lock(&mutex);
	gameLoop(localArgs->hrac, localArgs->event, localArgs->arrow_control);
	//pthread_mutex_unlock(&mutex);
	// cleanupSDL();
	return NULL;
}

void *vykreslovacieVlaknoFunc(void *arg)
{
	// while (1)
	// {
	// 	// pthread_mutex_lock(&mutex2);
    //     renderCell(hrac1);
	// 	//renderCell(hrac2);
    //     // pthread_mutex_unlock(&mutex2);
	// 	SDL_Delay(80);	
	// }
	renderCell(hrac1);
	while (1)
	{
		renderCell(hrac1);
		
		//SDL_Delay(80);
	}
	
	return NULL;
}

int main()
{
	pthread_t vykreslovacieVlakno, vlaknoHrac1, vlaknoHrac2;

	//pthread_mutex_init(&mutex, NULL);


	hrac1 = vytvorHraca("Kade", 5);
	hrac1->HADIK->x = 1;
	hrac1->HADIK->y = 1;
	hrac1->HADIK->snakeDirectionX = 1; // Initial direction (right)
	hrac1->HADIK->snakeDirectionY = 0;

	hrac2 = vytvorHraca("Marek", 5);
	hrac2->HADIK->x = 1;
	hrac2->HADIK->y = 3;
	hrac2->HADIK->snakeDirectionX = 1;
	hrac2->HADIK->snakeDirectionY = 0;

	SDL_Event event1;
	struct arguments *hrac1Args = malloc(sizeof(struct arguments));
	hrac1Args->event = &event1;
	hrac1Args->hrac = hrac1;
	hrac1Args->arrow_control = SDL_TRUE;

	SDL_Event event2;
	struct arguments* hrac2Args = malloc(sizeof(struct arguments));
	hrac2Args->event = &event2;
	hrac2Args->hrac = hrac2;
	hrac1Args->arrow_control = SDL_FALSE;



	initGame();
	initSnake(hrac1);
	// initSnake(hrac2);
	//randomFood();
	drawGameBoard();
	pthread_create(&vlaknoHrac1, NULL, playerThreadFunc, hrac1Args);
	pthread_create(&vlaknoHrac2, NULL, playerThreadFunc, hrac2Args);
	//pthread_create(&vykreslovacieVlakno, NULL, vykreslovacieVlaknoFunc, NULL);
	// gameLoop(hrac2);
	// pthread_create(&thread2, NULL, playerThread, hrac2Args);
	// initMenu();

	// menuLoop(hrac1);
	// cleanUpMenu();
	while (1)
	{
		// pthread_mutex_lock(&mutex2);
        renderCell(hrac1);
		renderCell(hrac2);
		//renderCell(hrac2);
        // pthread_mutex_unlock(&mutex2);
		//SDL_Delay(80);	
	}

	//pthread_join(vlaknoHrac1, NULL);

	//pthread_join(vykreslovacieVlakno, NULL);
	//pthread_mutex_destroy(&mutex);

	return 0;
}
