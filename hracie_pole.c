#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include "hracie_pole.h"
#include "hrac.h"
// #include "menu.h"

#define V_BUNKA 10
#define FRAME_RATE 80
#define MAX_SNAKE_SIZE 100

SDL_Window *windowGame = NULL;
SDL_Renderer *rendererGame = NULL;

SDL_Surface *apple_image_surface;
SDL_Texture *apple_image_texture;

SDL_Surface *grass_image_surface;
SDL_Texture *grass_image_texture;

SDL_Surface *snake_image_surface;
SDL_Texture *snake_image_texture;

SDL_Rect appleRect = {0, 0, V_BUNKA, V_BUNKA};

int quit = 0;

void randomFood()
{

	int foodX = rand() % (BOARD_SIZE_X / V_BUNKA);
	int foodY = rand() % (BOARD_SIZE_Y / V_BUNKA);

	while (gameBoard[foodX][foodY] == HAD)
	{
		foodX = rand() % BOARD_SIZE_X;
		foodY = rand() % BOARD_SIZE_Y;
	}

	gameBoard[foodX][foodY] = JEDLO;

	appleRect.x = foodX * V_BUNKA;
	appleRect.y = foodY * V_BUNKA;

	// SDL_RenderPresent(rendererGame);
}

void initSnake(HRAC *hrac)
{
	hrac->HADIK->snakeBodyX[0] = hrac->HADIK->x; // fix
	hrac->HADIK->snakeBodyY[0] = hrac->HADIK->y;

	for (int i = 0; i < hrac->HADIK->dlzka; i++)
	{
		gameBoard[hrac->HADIK->snakeBodyX[i]][hrac->HADIK->snakeBodyY[i]] = HAD;
	}
}

void initBoard()
{

	for (int i = 0; i < BOARD_SIZE_X; ++i)
	{
		for (int j = 0; j < BOARD_SIZE_Y; ++j)
		{
			gameBoard[i][j] = PRAZDNE;
		}
	}

	apple_image_surface = IMG_Load("images/apple_small.png");
	if (!apple_image_surface)
	{
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}
	apple_image_texture = SDL_CreateTextureFromSurface(rendererGame, apple_image_surface);
	if (!apple_image_texture)
	{
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}

	grass_image_surface = IMG_Load("images/grass.png");
	if (!grass_image_surface)
	{
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}
	grass_image_texture = SDL_CreateTextureFromSurface(rendererGame, grass_image_surface);
	if (!grass_image_texture)
	{
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}

	snake_image_surface = IMG_Load("images/snake_pattern.jpg");
	if (!snake_image_surface)
	{
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}
	snake_image_texture = SDL_CreateTextureFromSurface(rendererGame, snake_image_surface);
	if (!snake_image_texture)
	{
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}
}

void initGame()
{

	if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG)
	{
		fprintf(stderr, "Failed to initialize SDL_image: %s\n", IMG_GetError());
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", IMG_GetError());
	}

	windowGame = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_SIZE_X, BOARD_SIZE_Y, SDL_WINDOW_SHOWN);
	if (windowGame == NULL)
	{
		fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Log("Window could not be created! SDL_Error: %s\n", IMG_GetError());
	}

	rendererGame = SDL_CreateRenderer(windowGame, -1, SDL_RENDERER_ACCELERATED);
	if (rendererGame == NULL)
	{
		fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		SDL_Log("Renderer could not be created! SDL_Error: %s\n", IMG_GetError());
	}

	initBoard();
}

void drawGameBoard()
{
	SDL_RenderClear(rendererGame);
	for (int i = 0; i < BOARD_SIZE_X; ++i)
	{
		for (int j = 0; j < BOARD_SIZE_Y; ++j)
		{
			SDL_Rect cellRect = {i * V_BUNKA, j * V_BUNKA, V_BUNKA, V_BUNKA};
			switch (gameBoard[i][j])
			{
			case PRAZDNE:
				if (SDL_RenderCopy(rendererGame, grass_image_texture, NULL, &cellRect) != 0)
				{
					fprintf(stderr, "Failed to render image: %s\n", SDL_GetError());
				}
				break;
			case HAD:
				if (SDL_RenderCopy(rendererGame, snake_image_texture, NULL, &cellRect) != 0)
				{
					fprintf(stderr, "Failed to render image: %s\n", SDL_GetError());
				}
				break;
			case JEDLO:
				if (SDL_RenderCopy(rendererGame, apple_image_texture, NULL, &cellRect) != 0)
				{
					fprintf(stderr, "Failed to render image: %s\n", SDL_GetError());
				}
				break;
			}
		}
	}
	SDL_RenderPresent(rendererGame);
}

void cleanupSDL()
{
	if (apple_image_surface)
	{
		SDL_FreeSurface(apple_image_surface);
		apple_image_surface = NULL;
	}

	if (apple_image_texture)
	{
		SDL_DestroyTexture(apple_image_texture);
		apple_image_texture = NULL;
	}

	if (grass_image_surface)
	{
		SDL_FreeSurface(grass_image_surface);
		grass_image_surface = NULL;
	}

	if (grass_image_texture)
	{
		SDL_DestroyTexture(grass_image_texture);
		grass_image_texture = NULL;
	}

	if (snake_image_surface)
	{
		SDL_FreeSurface(snake_image_surface);
		snake_image_surface = NULL;
	}

	if (snake_image_texture)
	{
		SDL_DestroyTexture(snake_image_texture);
		snake_image_texture = NULL;
	}
	SDL_DestroyRenderer(rendererGame);
	SDL_DestroyWindow(windowGame);
	SDL_Quit();
}

void showMessage(const char *title, const char *message, SDL_MessageBoxFlags flags)
{
	SDL_ShowSimpleMessageBox(flags, title, message, NULL);
}

void showMessageBox(const char *title, const char *message, char *buttonText, HRAC *hrac)
{
	const SDL_MessageBoxButtonData buttons[] = {
		{SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, buttonText},
	};

	const SDL_MessageBoxData messageboxdata = {
		SDL_MESSAGEBOX_INFORMATION,
		windowGame, // Parent window, may be NULL
		title,
		message,
		SDL_arraysize(buttons),
		buttons,
		NULL // Default color scheme
	};

	int buttonid;
	SDL_ShowMessageBox(&messageboxdata, &buttonid);

	// Handle button press if needed
	// if (buttonid == -1 || buttonid == 0) {
	// 	quit = 1;
	// 	cleanupSDL(hrac);
	// 	initMenu();
	// 	menuLoop(hrac);
	// }
}

void renderCell(HRAC *hrac)
{
	SDL_Rect cellRect = {0, 0, V_BUNKA, V_BUNKA};
	cellRect.x = hrac->HADIK->x * V_BUNKA;
	cellRect.y = hrac->HADIK->y * V_BUNKA;
	if (SDL_RenderCopy(rendererGame, snake_image_texture, NULL, &cellRect) != 0)
	{
		fprintf(stderr, "Failed to render snake image: %s\n", SDL_GetError());
	}
	if (hrac->HADIK->snakeDirectionX == 1)
	{
	}
	cellRect.x = hrac->HADIK->previousTailX * V_BUNKA;
	cellRect.y = hrac->HADIK->previousTailY * V_BUNKA;
	SDL_Log("chvost na vymazanie x: %d, y: %d", hrac->HADIK->previousTailX, hrac->HADIK->previousTailY);
	if (SDL_RenderCopy(rendererGame, grass_image_texture, NULL, &cellRect) != 0)
	{
		fprintf(stderr, "Failed to render grass image: %s\n", SDL_GetError());
	}
	if (SDL_RenderCopy(rendererGame, apple_image_texture, NULL, &appleRect) != 0)
	{
		fprintf(stderr, "Failed to render apple image: %s\n", SDL_GetError());
	}
	SDL_RenderPresent(rendererGame);
}

void updateSnakePosition(HRAC *hrac)
{
	gameBoard[hrac->HADIK->snakeBodyX[hrac->HADIK->dlzka - 1]][hrac->HADIK->snakeBodyY[hrac->HADIK->dlzka - 1]] = PRAZDNE;
	hrac->HADIK->previousTailX = hrac->HADIK->snakeBodyX[hrac->HADIK->dlzka - 1];
	hrac->HADIK->previousTailY = hrac->HADIK->snakeBodyY[hrac->HADIK->dlzka - 1];

	for (int i = hrac->HADIK->dlzka - 1; i > 0; i--)
	{
		hrac->HADIK->snakeBodyX[i] = hrac->HADIK->snakeBodyX[i - 1];
		hrac->HADIK->snakeBodyY[i] = hrac->HADIK->snakeBodyY[i - 1];
	}

	// Update the head position
	int newSnakeX = hrac->HADIK->x + hrac->HADIK->snakeDirectionX;
	int newSnakeY = hrac->HADIK->y + hrac->HADIK->snakeDirectionY;

	// Check for collision with the walls
	if (newSnakeX < 0 || newSnakeX >= (BOARD_SIZE_X / V_BUNKA) || newSnakeY < 0 || newSnakeY >= (BOARD_SIZE_Y / V_BUNKA))
	{
		hrac->HADIK->snakeDirectionX = 0;
		hrac->HADIK->snakeDirectionY = 0;
		SDL_Log("Prehral si! Nabural si do steny");
		showMessageBox("Game Over", "Prehral si! Nabural si do steny", "Spat do menu", hrac);
		showMessage("Simple Message Box", "Hello from SDL!", SDL_MESSAGEBOX_INFORMATION);
	}
	else
	{
		// Check for collision with food
		if (gameBoard[newSnakeX][newSnakeY] == JEDLO)
		{
			hrac->HADIK->dlzka++;
			randomFood();
		}

		if (gameBoard[newSnakeX][newSnakeY] == HAD)
		{
			SDL_Log("Prehral si! Zjedol si hada");
			showMessageBox("Game Over", "Prehral si! Zjedol si hada", "Spat do menu", hrac);
			showMessage("Simple Message Box", "Hello from SDL!", SDL_MESSAGEBOX_INFORMATION);
		}
		//// Update the head position
		hrac->HADIK->x = newSnakeX;
		hrac->HADIK->y = newSnakeY;
		hrac->HADIK->snakeBodyX[0] = hrac->HADIK->x;
		hrac->HADIK->snakeBodyY[0] = hrac->HADIK->y;
		//// Update the board with the new snake positions
		for (int i = 0; i < hrac->HADIK->dlzka; i++)
		{
			gameBoard[hrac->HADIK->snakeBodyX[i]][hrac->HADIK->snakeBodyY[i]] = HAD;
		}
	}
}

void gameLoop(HRAC *hrac1, HRAC *hrac2, SDL_Event *event)
{
	while (!quit)
	{
		// Handle events
		while (SDL_PollEvent(event))
		{
			switch (event->type)
			{
			case SDL_QUIT:
				quit = 1;
				break;
			case SDL_KEYDOWN:
				if (event->key.keysym.sym == SDLK_q && (SDL_GetModState() & KMOD_CTRL) ||
					event->key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL))
				{
					// ctrl + q alebo ctrl + c, pre zrusenie hry
					quit = 1; // ToDo, toto hadze read access error

					cleanupSDL(hrac1);
					cleanupSDL(hrac2);
					// initMenu();
					// menuLoop(hrac);
				}
				else
				{
					switch (event->key.keysym.sym)
					{
					case SDLK_UP:
						if (hrac1->HADIK->snakeDirectionY != 1 || hrac1->HADIK->dlzka == 1) // ak nejde dole, moze ist hore
						{
							hrac1->HADIK->snakeDirectionX = 0;
							hrac1->HADIK->snakeDirectionY = -1;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}

						break;
					case SDLK_DOWN:
						if (hrac1->HADIK->snakeDirectionY != -1 || hrac1->HADIK->dlzka == 1)
						{
							hrac1->HADIK->snakeDirectionX = 0;
							hrac1->HADIK->snakeDirectionY = 1;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}
						break;
					case SDLK_LEFT:
						if (hrac1->HADIK->snakeDirectionX != 1 || hrac1->HADIK->dlzka == 1)
						{
							hrac1->HADIK->snakeDirectionX = -1;
							hrac1->HADIK->snakeDirectionY = 0;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}
						break;
					case SDLK_RIGHT:
						if (hrac1->HADIK->snakeDirectionX != -1 || hrac1->HADIK->dlzka == 1)
						{
							hrac1->HADIK->snakeDirectionX = 1;
							hrac1->HADIK->snakeDirectionY = 0;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}
						break;
					case SDLK_w:
						if (hrac2->HADIK->snakeDirectionY != 1 || hrac2->HADIK->dlzka == 1) // ak nejde dole, moze ist hore
						{
							hrac2->HADIK->snakeDirectionX = 0;
							hrac2->HADIK->snakeDirectionY = -1;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}

						break;
					case SDLK_s:
						if (hrac2->HADIK->snakeDirectionY != -1 || hrac2->HADIK->dlzka == 1)
						{
							hrac2->HADIK->snakeDirectionX = 0;
							hrac2->HADIK->snakeDirectionY = 1;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}
						break;
					case SDLK_a:
						if (hrac2->HADIK->snakeDirectionX != 1 || hrac2->HADIK->dlzka == 1)
						{
							hrac2->HADIK->snakeDirectionX = -1;
							hrac2->HADIK->snakeDirectionY = 0;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}
						break;
					case SDLK_d:
						if (hrac2->HADIK->snakeDirectionX != -1 || hrac2->HADIK->dlzka == 1)
						{
							hrac2->HADIK->snakeDirectionX = 1;
							hrac2->HADIK->snakeDirectionY = 0;
						}
						else
						{
							SDL_Log("Illegal move!");
							// printf("Illegal move!");
						}
						break;
					}

					// Handle arrow key events to change the snake's direction
				}
				break;
			}
		}
		if (!quit)
		{
			updateSnakePosition(hrac1);
			updateSnakePosition(hrac2);
			SDL_Delay(80);
		}
	}
}