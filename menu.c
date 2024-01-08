// #include "menu.h"
// #include "hracie_pole.h"
// #define MENU_SIZE_X 800
// #define MENU_SIZE_Y 800

// SDL_Window* windowMenu = NULL;
// SDL_Renderer* rendererMenu = NULL;
// Button playButton;
// Button quitButton;

// SDL_bool isPointInsideButton(Button* button, int x, int y)
// {
// 	return SDL_PointInRect(&(SDL_Point) { x, y }, & button->rect);
// }

// void initMenu() {

// 	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
// 		fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
// 	}

// 	windowMenu = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, MENU_SIZE_X, MENU_SIZE_Y, SDL_WINDOW_SHOWN);
// 	if (windowMenu == NULL) {
// 		fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
// 	}

// 	rendererMenu = SDL_CreateRenderer(windowMenu, -1, SDL_RENDERER_ACCELERATED);
// 	if (rendererMenu == NULL) {
// 		fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
// 	}

// 	if (TTF_Init() < 0) {
// 		fprintf(stderr, "TTF could not initialize! SDL_Error: %s\n", TTF_GetError());
// 	}
// }

// void renderButton(int x, int y, int width, int height, Button* button, TTF_Font* font, SDL_Color textColor, const char* buttonText) {
// 	SDL_Surface* textSurface;
// 	SDL_Texture* textTexture;

// 	button->rect = (SDL_Rect){ x, y, width, height };
// 	button->color = (SDL_Color){ 211,211,211, 255 };
// 	button->isClicked = SDL_FALSE;

// 	SDL_SetRenderDrawColor(rendererMenu, button->color.r, button->color.g, button->color.b, button->color.a);
// 	SDL_RenderFillRect(rendererMenu, &(button->rect));
// 	textSurface = TTF_RenderUTF8_Blended(font, buttonText, textColor);
// 	textTexture = SDL_CreateTextureFromSurface(rendererMenu, textSurface);
// 	SDL_RenderCopy(rendererMenu, textTexture, NULL, &(button->rect));

// 	SDL_FreeSurface(textSurface);
// 	SDL_DestroyTexture(textTexture);
// }

// void renderMenu() {
// 	SDL_SetRenderDrawColor(rendererMenu, 255, 255, 255, 255);
// 	SDL_RenderClear(rendererMenu);

// 	// Render menu items (for simplicity, just rendering text here)
// 	SDL_Color textColor = { 0, 0, 0, 255 };


// 	TTF_Font* font = TTF_OpenFont("arial-unicode-ms.ttf", 28);
// 	if (font == NULL) {
// 		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
// 	}

// 	SDL_Surface* imageSurface = IMG_Load("images/snake_logo.png");
// 	if (!imageSurface) {
// 		printf("Image not found");
// 	}
// 	SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(rendererMenu, imageSurface);
// 	SDL_Rect imageRect = { MENU_SIZE_X / 2, 10, 300, 300 };
// 	SDL_RenderCopy(rendererMenu, imageTexture, NULL, &imageRect);
// 	SDL_FreeSurface(imageSurface);
// 	SDL_DestroyTexture(imageTexture);

// 	renderButton(MENU_SIZE_X / 2 - 50, 100, 100, 50, &playButton, font, textColor, "Play game");
// 	renderButton(MENU_SIZE_X / 2 - 50, 200, 100, 50, &quitButton, font, textColor, "Quit");

// 	TTF_CloseFont(font);

// 	// Present the rendered items
// 	SDL_RenderPresent(rendererMenu);
// }

// void menuLoop(HRAC* hrac) {
// 	SDL_Event event;
// 	int quit = 0;
// 	int gameState = MENU;

// 	while (!quit) {

// 		while (SDL_PollEvent(&event) != 0) {

// 			if (event.type == SDL_QUIT) {
// 				quit = 1;
// 			}
// 			else if (event.type == SDL_MOUSEBUTTONDOWN) {
// 				if (isPointInsideButton(&playButton, event.button.x, event.button.y))
// 				{
// 					playButton.isClicked = SDL_TRUE;
// 					gameState = GAME;
// 				}
// 				else if (isPointInsideButton(&quitButton, event.button.x, event.button.y)) {
// 					quitButton.isClicked = SDL_TRUE;
// 					gameState = QUIT;
// 				}
// 			}
// 			else if (event.type == SDL_MOUSEBUTTONUP) {
// 				playButton.isClicked = SDL_FALSE;
// 				quitButton.isClicked = SDL_FALSE;
// 			}
// 			if (event.key.keysym.sym == SDLK_q && (SDL_GetModState() & KMOD_CTRL) ||
// 				event.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL))
// 			{
// 				quitButton.isClicked = SDL_TRUE;
// 				gameState = QUIT;
// 			}
// 		}
// 		if (gameState == MENU) {
// 			renderMenu(rendererMenu);
// 		}
// 		else if (gameState == GAME) {
// 			quit = 1;
// 			cleanUpMenu();
// 			initGame(hrac);
// 			gameLoop(hrac);
// 		}
// 		else if (gameState == QUIT) {
// 			quit = 1;
// 			cleanUpMenu();
// 		}
// 	}
// }

// void renderTextField(const char* text, int x, int y) {
	
// 	TTF_Font* font = TTF_OpenFont("arial-unicode-ms.ttf", 28);
// 	if (font == NULL) {
// 		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
// 	}

// 	SDL_Color textColor = { 255, 255, 255, 255 };
// 	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, textColor);
// 	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(rendererMenu, textSurface);

// 	int textWidth = textSurface->w;
// 	int textHeight = textSurface->h;

// 	SDL_Rect renderQuad = { x, y, textWidth, textHeight };
// 	SDL_RenderCopy(rendererMenu, textTexture, NULL, &renderQuad);

// 	SDL_FreeSurface(textSurface);
// 	SDL_DestroyTexture(textTexture);

// 	TTF_CloseFont(font);
// }

// void cleanUpMenu()
// {
// 	SDL_DestroyRenderer(rendererMenu);
// 	SDL_DestroyWindow(windowMenu);
// 	SDL_Quit();
// }