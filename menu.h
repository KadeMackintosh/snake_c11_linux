#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "hrac.h"


typedef struct {
    SDL_Rect rect;        // Button's position and size
    SDL_Color color;      // Button's color
    SDL_bool isClicked;   // Flag to indicate if the button is clicked
} Button;

enum GameState {
	MENU,
	GAME,
	QUIT,
};

SDL_bool isPointInsideButton(Button* button, int x, int y);
void initMenu();
void renderMenu();
void menuLoop(HRAC* hrac);
void cleanUpMenu();