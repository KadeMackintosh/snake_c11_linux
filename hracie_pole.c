#include "hracie_pole.h"


#define V_BUNKA 10
#define FRAME_RATE 80
#define MAX_SNAKE_SIZE 100

SDL_Window *windowGame = NULL;
SDL_Renderer *rendererGame = NULL;

SDL_Surface *apple_image_surface;
SDL_Texture *apple_image_texture;

SDL_Surface *grass_image_surface;
SDL_Texture *grass_image_texture;

//snake_image_surface je v hracovi priamo
//SDL_Texture *snake_image_texture;
SDL_Surface *snake1_image_surface;
SDL_Texture *snake1_image_texture;

SDL_Surface *snake2_image_surface;
SDL_Texture *snake2_image_texture;

SDL_Rect appleRect = {0, 0, V_BUNKA, V_BUNKA};

int quit = 0;

void randomFood()
{

	int foodX = rand() % (BOARD_SIZE_X / V_BUNKA);
	int foodY = rand() % (BOARD_SIZE_Y / V_BUNKA);

	while ((gameBoard[foodX][foodY] == HAD1) || (gameBoard[foodX][foodY] == HAD2))
	{
		foodX = rand() % BOARD_SIZE_X;
		foodY = rand() % BOARD_SIZE_Y;
	}

	gameBoard[foodX][foodY] = JEDLO;

	appleRect.x = foodX * V_BUNKA;
	appleRect.y = foodY * V_BUNKA;

	// SDL_RenderPresent(rendererGame);
}

void initSnake(HRAC *hrac1, HRAC *hrac2)
{
	hrac1->HADIK->snakeBodyX[0] = hrac1->HADIK->x;
	hrac1->HADIK->snakeBodyY[0] = hrac1->HADIK->y;
	for (int i = 0; i < hrac1->HADIK->dlzka; i++)
	{
		gameBoard[hrac1->HADIK->snakeBodyX[i]][hrac1->HADIK->snakeBodyY[i]] = HAD1;
	}
	hrac2->HADIK->snakeBodyX[0] = hrac2->HADIK->x;
	hrac2->HADIK->snakeBodyY[0] = hrac2->HADIK->y;
	for (int i = 0; i < hrac2->HADIK->dlzka; i++)
	{
		gameBoard[hrac2->HADIK->snakeBodyX[i]][hrac2->HADIK->snakeBodyY[i]] = HAD2;
	}

	// snake1_image_surface=hrac1->snake_image_surface;
	// snake2_image_surface= hrac2->snake_image_surface;
}

void initBoard(HRAC* hrac1, HRAC* hrac2)
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


	snake1_image_texture = SDL_CreateTextureFromSurface(rendererGame, hrac1->snake_image_surface);

	if (!snake1_image_texture)
	{
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}

	snake2_image_texture = SDL_CreateTextureFromSurface(rendererGame, hrac2->snake_image_surface);
	if (!snake2_image_texture)
	{
		SDL_Log("Failed to initialize SDL_image: %s\n", IMG_GetError());
	}
}

void initGame(HRAC* hrac1, HRAC* hrac2)
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

	if (TTF_Init() < 0)
	{
		fprintf(stderr, "TTF could not initialize! TTF_Error: %s\n", SDL_GetError());
	}

	windowGame = SDL_CreateWindow(hrac1->meno, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, BOARD_SIZE_X, BOARD_SIZE_Y, SDL_WINDOW_SHOWN);
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

	initBoard(hrac1, hrac2);
}

void drawGameBoard(HRAC* hrac1, HRAC* hrac2)
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
			case HAD1:
				if (SDL_RenderCopy(rendererGame, snake1_image_texture, NULL, &cellRect) != 0)
				{
					fprintf(stderr, "Failed to render snake1_image_texture image: %s\n", SDL_GetError());
				}
				break;
			case HAD2:
				if (SDL_RenderCopy(rendererGame, snake2_image_texture, NULL, &cellRect) != 0)
				{
					fprintf(stderr, "Failed to render  snake2_image_texture image: %s\n", SDL_GetError());
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

	if (snake1_image_surface)
	{
		SDL_FreeSurface(snake1_image_surface);
		snake1_image_surface = NULL;
	}

	if (snake1_image_texture)
	{
		SDL_DestroyTexture(snake1_image_texture);
		snake1_image_texture = NULL;
	}

	if (snake2_image_surface)
	{
		SDL_FreeSurface(snake2_image_surface);
		snake2_image_surface = NULL;
	}

	if (snake2_image_texture)
	{
		SDL_DestroyTexture(snake2_image_texture);
		snake2_image_texture = NULL;
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

void gameOver(HRAC* hrac) {
	TTF_Font* font = TTF_OpenFont("arial-unicode-ms.ttf", 20);
	if (font == NULL) {
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
	}

	SDL_Surface* textSurface;
	SDL_Texture* textTexture;
	SDL_Rect textRect = {0, BOARD_SIZE_Y / 4, 800, 400};
	SDL_Color color = { 255,0,0, 255 };

	SDL_SetRenderDrawColor(rendererGame, color.r, color.g, color.b, color.a);
	char gameOverMessage[100] = "Hra skončila, vyhral: ";
	strncat(gameOverMessage, hrac->meno, strlen(hrac->meno));
	textSurface = TTF_RenderUTF8_Blended(font, gameOverMessage, color);
	textTexture = SDL_CreateTextureFromSurface(rendererGame, textSurface);
	if (textTexture == NULL)
	{
		printf("Failed to create texture! SDL Error: %s\n", SDL_GetError());
	}
	SDL_RenderCopy(rendererGame, textTexture, NULL, &textRect);
	SDL_RenderPresent(rendererGame);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}

void renderCellH1(HRAC *hrac1)
{
	SDL_Rect cellRect = {0, 0, V_BUNKA, V_BUNKA};
	cellRect.x = hrac1->HADIK->x * V_BUNKA;
	cellRect.y = hrac1->HADIK->y * V_BUNKA;
	if (SDL_RenderCopy(rendererGame, snake1_image_texture, NULL, &cellRect) != 0)  
	{
		fprintf(stderr, "Failed to render snake1_image_texture image: %s\n", SDL_GetError());
	}
	if (hrac1->HADIK->snakeDirectionX == 1)
	{
	}
	cellRect.x = hrac1->HADIK->previousTailX * V_BUNKA;
	cellRect.y = hrac1->HADIK->previousTailY * V_BUNKA;
	//SDL_Log("chvost na vymazanie x: %d, y: %d", hrac->HADIK->previousTailX, hrac->HADIK->previousTailY);
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

void renderCellH2(HRAC *hrac2)
{
	SDL_Rect cellRect = {0, 0, V_BUNKA, V_BUNKA};
	cellRect.x = hrac2->HADIK->x * V_BUNKA;
	cellRect.y = hrac2->HADIK->y * V_BUNKA;
	if (SDL_RenderCopy(rendererGame, snake2_image_texture, NULL, &cellRect) != 0)  //////////// !! snake1_image_texture FIX!
	{
		fprintf(stderr, "Failed to render snake2_image_texture image: %s\n", SDL_GetError());
	}
	if (hrac2->HADIK->snakeDirectionX == 1)
	{
	}
	cellRect.x = hrac2->HADIK->previousTailX * V_BUNKA;
	cellRect.y = hrac2->HADIK->previousTailY * V_BUNKA;
	//SDL_Log("chvost na vymazanie x: %d, y: %d", hrac->HADIK->previousTailX, hrac->HADIK->previousTailY);
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

void updateSnakePosition(HRAC *hrac, enum CellType pHAD)
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
		quit = 1;
		hrac->hracVyhral = -1;
		// SDL_Delay(80);
		// gameOver(hrac);
		//cleanupSDL(hrac);
	}
	else
	{
		// Check for collision with food
		if (gameBoard[newSnakeX][newSnakeY] == JEDLO)
		{
			hrac->HADIK->dlzka++;
			randomFood();
		}

		if ((gameBoard[newSnakeX][newSnakeY] == HAD1) || (gameBoard[newSnakeX][newSnakeY] == HAD2))
		{
			//SDL_Log("Prehral si! Zjedol si hada");
			quit = 1;
			hrac->hracVyhral = -1;
			
		}
		//// Update the head position
		hrac->HADIK->x = newSnakeX;
		hrac->HADIK->y = newSnakeY;
		hrac->HADIK->snakeBodyX[0] = hrac->HADIK->x;
		hrac->HADIK->snakeBodyY[0] = hrac->HADIK->y;
		//// Update the board with the new snake positions
		for (int i = 0; i < hrac->HADIK->dlzka; i++)
		{
			gameBoard[hrac->HADIK->snakeBodyX[i]][hrac->HADIK->snakeBodyY[i]] = pHAD;
		}
	}
}

void gameLoop(HRAC *hrac1, HRAC *hrac2)
{
	while (!quit)
	{
		updateSnakePosition(hrac1, HAD1);
		updateSnakePosition(hrac2, HAD2);
		SDL_Delay(FRAME_RATE);
	}
	if (hrac1->hracVyhral == -1)
	{
		hrac2->hracVyhral = 1;
	}else if(hrac2->hracVyhral == -1){
		hrac1->hracVyhral = 1;
	}
	

}