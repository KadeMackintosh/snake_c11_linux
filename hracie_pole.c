#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "hracie_pole.h"
#include "hrac.h"
#include "menu.h"

#define V_BUNKA 10
#define FRAME_RATE 200
#define MAX_HRACI 2
SDL_Window* windowGame = NULL;
SDL_Renderer* rendererGame = NULL;
#define MAX_SNAKE_SIZE 100

// Load the image only once outside the loop
SDL_Surface* imageSurface;
SDL_Texture* imageTexture;
SDL_Rect imageRect = { 0, 0, V_BUNKA, V_BUNKA };

HRAC* poleHracov[MAX_HRACI];

int quit = 0;

void randomFood() {
	SDL_Rect cellRect;
	int foodX = rand() % (BOARD_SIZE_X / V_BUNKA);
	int foodY = rand() % (BOARD_SIZE_Y / V_BUNKA);

	while (gameBoard[foodX][foodY] == HAD) {
		foodX = rand() % BOARD_SIZE_X;
		foodY = rand() % BOARD_SIZE_Y;
	}
	//printf("foodX: %d, foodY: %d\n", foodX, foodY);

	gameBoard[foodX][foodY] = JEDLO;
	SDL_SetRenderDrawColor(rendererGame, 255, 211, 30, 255);
	cellRect.x = foodX * V_BUNKA;
	cellRect.y = foodY * V_BUNKA;
	cellRect.h = V_BUNKA;
	cellRect.w = V_BUNKA;
	SDL_RenderFillRect(rendererGame, &cellRect);
	SDL_RenderPresent(rendererGame);
}

void initBoard(HRAC* hrac) {

	//poleHracov[1] = vytvorHraca("Kade", 4);

	hrac->HADIK->x = 1;
	hrac->HADIK->y = 1;

	hrac->HADIK->snakeBodyX[0] = hrac->HADIK->x; //fix
	hrac->HADIK->snakeBodyY[0] = hrac->HADIK->y;

	hrac->HADIK->snakeDirectionX = 1; // Initial direction (right)
	hrac->HADIK->snakeDirectionY = 0;

	for (int i = 0; i < BOARD_SIZE_X; ++i) {
		for (int j = 0; j < BOARD_SIZE_Y; ++j) {
			gameBoard[i][j] = PRAZDNE;
		}
	}
	// Load the image only once outside the loop
	imageSurface = IMG_Load("images/apple_small.png");
	if (!imageSurface) {
		//printf("Failed to load image: %s\n", IMG_GetError());
	}
	imageTexture = SDL_CreateTextureFromSurface(rendererGame, imageSurface);
	if (!imageTexture) {
		//printf("Failed to create texture: %s\n", SDL_GetError());
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
		//fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		//fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
	}

	windowGame = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_SIZE_X, BOARD_SIZE_Y, SDL_WINDOW_SHOWN);
	if (windowGame == NULL) {
		//fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	rendererGame = SDL_CreateRenderer(windowGame, -1, SDL_RENDERER_ACCELERATED);
	if (rendererGame == NULL) {
		//fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
	}

	initBoard(hrac);
}

void drawGameBoard() {
	// clear
	SDL_RenderClear(rendererGame);
	for (int i = 0; i < BOARD_SIZE_X; ++i) {
		for (int j = 0; j < BOARD_SIZE_Y; ++j) {
			SDL_Rect cellRect = { i * V_BUNKA, j * V_BUNKA, V_BUNKA, V_BUNKA };
			SDL_SetRenderDrawColor(rendererGame, 0, 0, 0, 255);    // farba buniek
			enum CellType previousCell = gameBoard[i][j];
			switch (gameBoard[i][j]) {
			case PRAZDNE:
				SDL_SetRenderDrawColor(rendererGame, 144, 238, 144, 255);  // farba prazdnych buniek
				break;
			case HAD:
				SDL_SetRenderDrawColor(rendererGame, 0, 0, 0, 255); // farba hada
				break;
			case JEDLO:
				SDL_SetRenderDrawColor(rendererGame, 255, 211, 30, 255);
				//imageRect.x = i * V_BUNKA;
				//imageRect.y = j * V_BUNKA;
				//if (SDL_RenderCopy(rendererGame, imageTexture, NULL, &imageRect) != 0) {
				//    fprintf(stderr, "Failed to render image: %s\n", SDL_GetError());
				//}
				break;
			}

			SDL_RenderFillRect(rendererGame, &cellRect);
		}
	}
	SDL_RenderPresent(rendererGame);
}

void cleanupSDL() {
	if (imageSurface) {
		SDL_FreeSurface(imageSurface);
		imageSurface = NULL;
	}

	if (imageTexture) {
		SDL_DestroyTexture(imageTexture);
		imageTexture = NULL;
	}
	SDL_DestroyRenderer(rendererGame);
	SDL_DestroyWindow(windowGame);
	SDL_Quit();
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
	SDL_SetRenderDrawColor(rendererGame, 144, 238, 144, 255);
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
		showMessageBox("Game Over", "Prehral si! Nabural si do steny", "Spat do menu", hrac);
	}
	else {
		// Check for collision with food
		if (gameBoard[newSnakeX][newSnakeY] == JEDLO) {
			hrac->HADIK->dlzka++;
			randomFood();
		}

		if (gameBoard[newSnakeX][newSnakeY] == HAD) {
			showMessageBox("Game Over", "Prehral si! Zjedol si hada", "Spat do menu", hrac);
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
							printf("Illegal move!");
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
							printf("Illegal move!");
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
							printf("Illegal move!");
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
							printf("Illegal move!");
						}
						break;
					}
				}
				break;
			}
		}
		// Update snake position
		updateSnakePosition(hrac);
		//drawGameBoard();
		//printf("Hrac: %s snakeX: %d, snakeY: %d\n", hrac->meno, hrac->HADIK->x, hrac->HADIK->y);
		SDL_Delay(FRAME_RATE);
	}
}