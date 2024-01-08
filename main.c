#include <stdio.h>
#include <SDL.h>
#include <pthread.h>
#include "menu.h"
#include "hrac.h"

int main() {
	//pthread_t thread1, thread/*2*/, drawBoardThread; // You can extend this for more players

	HRAC* hrac1 = vytvorHraca("Kade", 1);
	hrac1->HADIK->x = 1;
	hrac1->HADIK->y = 1;
	hrac1->HADIK->snakeDirectionX = 1; // Initial direction (right)
	hrac1->HADIK->snakeDirectionY = 0;
    
	/*HRAC* hrac2 = vytvorHraca("Cundo", 1);
	hrac2->HADIK->x = 700;
	hrac2->HADIK->y = 700;
	hrac2->HADIK->snakeDirectionX = -1;
	hrac2->HADIK->snakeDirectionY = 0;
*/
	initMenu();
	menuLoop(hrac1);
	cleanUpMenu();

	return 0;
}
