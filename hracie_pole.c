﻿#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "hracie_pole.h"
#include "hrac.h"
#include "menu.h"

#define V_BUNKA 10
#define FRAME_RATE 60
#define MAX_HRACI 2
SDL_Window* windowGame = NULL;
SDL_Renderer* rendererGame = NULL;
#define MAX_SNAKE_SIZE 100

SDL_Surface* apple_image_surface;
SDL_Texture* apple_image_texture;
SDL_Surface* grass_image_surface;
SDL_Texture* grass_image_texture;
// SDL_Surface* snake_image_surface;
// SDL_Texture* snake_image_texture;

SDL_Rect imageRect = { 0, 0, V_BUNKA, V_BUNKA };

// SDL_Rect appleImageRect = { 0, 0, V_BUNKA, V_BUNKA };
// SDL_Rect grassImageRect = { 0, 0, V_BUNKA, V_BUNKA };
// SDL_Rect snakeImageRect = { 0, 0, V_BUNKA, V_BUNKA };

HRAC* poleHracov[MAX_HRACI];

int quit = 0;

void randomFood() {
    int foodX = rand() % (BOARD_SIZE_X / V_BUNKA);
    int foodY = rand() % (BOARD_SIZE_Y / V_BUNKA);

    while (gameBoard[foodX][foodY] == HAD) {
        foodX = rand() % BOARD_SIZE_X;
        foodY = rand() % BOARD_SIZE_Y;
    }

    gameBoard[foodX][foodY] = JEDLO;

    imageRect.x = foodX * V_BUNKA;
    imageRect.y = foodY * V_BUNKA;

    if (SDL_RenderCopy(rendererGame, apple_image_texture, NULL, &imageRect) != 0) {
        fprintf(stderr, "Failed to render image: %s\n", SDL_GetError());
    }
	SDL_RenderPresent(rendererGame);
}


void initBoard(HRAC* hrac) {

	hrac->HADIK->snakeBodyX[0] = hrac->HADIK->x;
	hrac->HADIK->snakeBodyY[0] = hrac->HADIK->y;

	for (int i = 0; i < BOARD_SIZE_X; ++i) {
		for (int j = 0; j < BOARD_SIZE_Y; ++j) {
			gameBoard[i][j] = PRAZDNE;
		}
	}

	apple_image_surface = IMG_Load("images/apple_small.png");
	if (!apple_image_surface) {
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}
	apple_image_texture = SDL_CreateTextureFromSurface(rendererGame, apple_image_surface);
	if (!apple_image_texture) {
		SDL_Log("Failed to create texture: %s\n", IMG_GetError());
	}

	grass_image_surface = IMG_Load("images/grass.png");
	if (!grass_image_surface) {
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}
	grass_image_texture = SDL_CreateTextureFromSurface(rendererGame, grass_image_surface);
	if (!grass_image_surface) {
		SDL_Log("Failed to create texture: %s\n", IMG_GetError());
	}

	randomFood();

	for (int i = 0; i < hrac->HADIK->dlzka; i++) {
		gameBoard[hrac->HADIK->snakeBodyX[i]][hrac->HADIK->snakeBodyY[i]] = HAD;
	}
	drawGameBoard();
}

void initGame(HRAC* hrac) {

	// Initialize SDL_image
	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
		fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", IMG_GetError());
	}

	windowGame = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_SIZE_X, BOARD_SIZE_Y, SDL_WINDOW_SHOWN);
	if (windowGame == NULL) {
		fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Log("Window could not be created! SDL_Error: %s\n", IMG_GetError());
	}

	rendererGame = SDL_CreateRenderer(windowGame, -1, SDL_RENDERER_ACCELERATED);
	if (rendererGame == NULL) {
		fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Log("Renderer could not be created! SDL_Error: %s\n", IMG_GetError());
	}

	initBoard(hrac);
}

void drawGameBoard() {
	for (int i = 0; i < BOARD_SIZE_X; ++i) {
		for (int j = 0; j < BOARD_SIZE_Y; ++j) {
			SDL_Rect cellRect = { i * V_BUNKA, j * V_BUNKA, V_BUNKA, V_BUNKA };
			switch (gameBoard[i][j]) {
			case PRAZDNE:
				// SDL_SetRenderDrawColor(rendererGame, 144, 238, 144, 255);  // farba prazdnych buniek
				cellRect.x = i * V_BUNKA;
				cellRect.y = j * V_BUNKA;
				cellRect.h = V_BUNKA;
				cellRect.w = V_BUNKA;
				if (SDL_RenderCopy(rendererGame, grass_image_texture, NULL, &cellRect) != 0) {
				   fprintf(stderr, "Failed to render image: %s\n", SDL_GetError());
				}
				break;
			case HAD:
				SDL_SetRenderDrawColor(rendererGame, 0, 0, 0, 255); // farba hada
				break;
			case JEDLO:
				// SDL_SetRenderDrawColor(rendererGame, 255, 211, 30, 255);
				cellRect.x = i * V_BUNKA;
				cellRect.y = j * V_BUNKA;
				cellRect.h = V_BUNKA;
				cellRect.w = V_BUNKA;
				if (SDL_RenderCopy(rendererGame, apple_image_texture, NULL, &cellRect) != 0) {
				   fprintf(stderr, "Failed to render image: %s\n", SDL_GetError());
				}
				break;
			}
			SDL_RenderFillRect(rendererGame, &cellRect);
		}
	}
	SDL_RenderPresent(rendererGame);
}

void cleanupSDL() {
	if (apple_image_surface) {
		SDL_FreeSurface(apple_image_surface);
		apple_image_surface = NULL;
	}

	if (apple_image_texture) {
		SDL_DestroyTexture(apple_image_texture);
		apple_image_texture = NULL;
	}
	SDL_DestroyRenderer(rendererGame);
	SDL_DestroyWindow(windowGame);
	SDL_Quit();
}
void showMessage(const char* title, const char* message, SDL_MessageBoxFlags flags) {
    SDL_ShowSimpleMessageBox(flags, title, message, NULL);
}
void showMessageBox(const char* title, const char* message, char* buttonText, HRAC* hrac) {
	const SDL_MessageBoxButtonData buttons[] = {
		{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, buttonText },
	};

	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,
		windowGame,  // Parent window, may be NULL
		title,
		message,
		SDL_arraysize(buttons),
		buttons,
		NULL // Default color scheme
	};

	int buttonid;
	SDL_ShowMessageBox(&messageboxdata, &buttonid);

	// Handle button press if needed
	if (buttonid == -1 || buttonid == 0) {
		quit = 1;
		cleanupSDL(hrac);
		initMenu();
		menuLoop(hrac);
	}
}


void updateSnakePosition(HRAC* hrac) {
	SDL_Rect cellRect;
	// Clear the tail position
	gameBoard[hrac->HADIK->snakeBodyX[hrac->HADIK->dlzka - 1]][hrac->HADIK->snakeBodyY[hrac->HADIK->dlzka - 1]] = PRAZDNE;
	cellRect.x = hrac->HADIK->snakeBodyX[hrac->HADIK->dlzka - 1] * V_BUNKA;
	cellRect.y = hrac->HADIK->snakeBodyY[hrac->HADIK->dlzka - 1] * V_BUNKA;
	cellRect.h = V_BUNKA;
	cellRect.w = V_BUNKA;
	if (SDL_RenderCopy(rendererGame, grass_image_texture, NULL, &cellRect) != 0) {
		fprintf(stderr, "Failed to render image: %s\n", SDL_GetError());
	}
	// SDL_SetRenderDrawColor(rendererGame, 144, 238, 144, 255); //
	SDL_RenderFillRect(rendererGame, &cellRect);

	// Update the snake body positions
	for (int i = hrac->HADIK->dlzka - 1; i > 0; i--) {
		hrac->HADIK->snakeBodyX[i] = hrac->HADIK->snakeBodyX[i - 1];
		hrac->HADIK->snakeBodyY[i] = hrac->HADIK->snakeBodyY[i - 1];
	}

	// Update the head position
	int newSnakeX = hrac->HADIK->x + hrac->HADIK->snakeDirectionX;
	int newSnakeY = hrac->HADIK->y + hrac->HADIK->snakeDirectionY;

	// Check for collision with the walls
	if (newSnakeX < 0 || newSnakeX >= (BOARD_SIZE_X / V_BUNKA) || newSnakeY < 0 || newSnakeY >= (BOARD_SIZE_Y / V_BUNKA)) {
		hrac->HADIK->snakeDirectionX = 0;
		hrac->HADIK->snakeDirectionY = 0;
		SDL_Log("Prehral si! Nabural si do steny");
		// showMessageBox("Game Over", "Prehral si! Nabural si do steny", "Spat do menu", hrac);
		showMessage("Simple Message Box", "Hello from SDL!", SDL_MESSAGEBOX_INFORMATION);
	}
	else {
		// Check for collision with food
		if (gameBoard[newSnakeX][newSnakeY] == JEDLO) {
			hrac->HADIK->dlzka++;
			randomFood();
		}

		if (gameBoard[newSnakeX][newSnakeY] == HAD) {
			SDL_Log("Prehral si! Zjedol si hada");
			// showMessageBox("Game Over", "Prehral si! Zjedol si hada", "Spat do menu", hrac);
			showMessage("Simple Message Box", "Hello from SDL!", SDL_MESSAGEBOX_INFORMATION);
		}
		//// Update the head position
		hrac->HADIK->x = newSnakeX;
		hrac->HADIK->y = newSnakeY;
		hrac->HADIK->snakeBodyX[0] = hrac->HADIK->x;
		hrac->HADIK->snakeBodyY[0] = hrac->HADIK->y;

		//// Update the board with the new snake positions
		for (int i = 0; i < hrac->HADIK->dlzka; i++) {
			gameBoard[hrac->HADIK->snakeBodyX[i]][hrac->HADIK->snakeBodyY[i]] = HAD;
			SDL_SetRenderDrawColor(rendererGame, 0, 0, 0, 255);
			cellRect.x = hrac->HADIK->snakeBodyX[i] * V_BUNKA;
			cellRect.y = hrac->HADIK->snakeBodyY[i] * V_BUNKA;
			cellRect.w = V_BUNKA;
			cellRect.h = V_BUNKA;
			SDL_RenderFillRect(rendererGame, &cellRect);
		}
	}
	SDL_RenderPresent(rendererGame);
}

void gameLoop(HRAC* hrac) {
	SDL_Event event;
	while (!quit) {
		// Handle events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_q && (SDL_GetModState() & KMOD_CTRL) ||
					event.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL)) {
					// ctrl + q alebo ctrl + c, pre zrusenie hry
					quit = 1;  //ToDo, toto hadze read access error
					cleanupSDL(hrac);
					initMenu();
					menuLoop(hrac);
				}
				else {
					// Handle arrow key events to change the snake's direction
					switch (event.key.keysym.sym) {
					case SDLK_UP:
						if (hrac->HADIK->snakeDirectionY != 1 || hrac->HADIK->dlzka == 1) //ak nejde dole, moze ist hore
						{
							hrac->HADIK->snakeDirectionX = 0;
							hrac->HADIK->snakeDirectionY = -1;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}

						break;
					case SDLK_DOWN:
						if (hrac->HADIK->snakeDirectionY != -1 || hrac->HADIK->dlzka == 1)
						{
							hrac->HADIK->snakeDirectionX = 0;
							hrac->HADIK->snakeDirectionY = 1;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}
						break;
					case SDLK_LEFT:
						if (hrac->HADIK->snakeDirectionX != 1 || hrac->HADIK->dlzka == 1)
						{
							hrac->HADIK->snakeDirectionX = -1;
							hrac->HADIK->snakeDirectionY = 0;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}
						break;
					case SDLK_RIGHT:
						if (hrac->HADIK->snakeDirectionX != -1 || hrac->HADIK->dlzka == 1)
						{
							hrac->HADIK->snakeDirectionX = 1;
							hrac->HADIK->snakeDirectionY = 0;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}
						break;
					}
				}
				break;
			}
		}
		// Update snake position
		updateSnakePosition(hrac);
		//printf("Hrac: %s snakeX: %d, snakeY: %d\n", hrac->meno, hrac->HADIK->x, hrac->HADIK->y);
		SDL_Delay(FRAME_RATE);
	}
}