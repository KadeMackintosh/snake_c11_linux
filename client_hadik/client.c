#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()
#include <pthread.h>
#include <SDL2/SDL.h>
#include "client_hracie_pole.h"
#include "client_hrac.h"
#define MAX 80
#define PORT 8080
#define SA struct sockaddr


struct sendArguments{
	int *sockfd;
	SDL_Event *event;
};

HRAC *hrac1;
HRAC *hrac2;
int quitGraphics = 0;

void sendFunc(int sockfd, SDL_Event event) {
	

    // Create an SDL window
    
    char buff[MAX];

    while(!quitGraphics) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_KEYDOWN) {
                // Handle key presses here
                switch (event.key.keysym.sym) {
                    case SDLK_w:
                        strcpy(buff, "w");
                        break;
                    case SDLK_a:
                        strcpy(buff, "a");
                        break;
                    case SDLK_s:
                        strcpy(buff, "s");
                        break;
                    case SDLK_d:
                        strcpy(buff, "d");
                        break;
					case SDLK_q:
						strcpy(buff, "q");
						break;
                    default:
                        // Ignore other keys
                        continue;  // Skip the rest of the loop for non-matching keys
                }

                // Send the message to the server immediately after key press
                write(sockfd, buff, sizeof(buff));
				if(strncmp(buff, "q", 1) == 0) {
					quitGraphics = 1;
					SDL_Log("Exiting client by client...");
					//cleanupSDL();
					//exit(0);
					break;
				}
				sendSignalToTurn(hrac2, buff);
            }
        }
    }
	printf("exited for loop in sendFunc on client");
}


void receiveFunc(int sockfd)
{
	char buff[MAX];
	int n;
	while(!quitGraphics) {
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s\n", buff);
		if (strncmp(buff, "q", 1) == 0)
		{
			quitGraphics = 1;
			SDL_Log("Exiting client by server...");
			//cleanupSDL();
			//exit(0);
			break;
		}
		sendSignalToTurn(hrac1, buff);
		SDL_Delay(80);
		bzero(buff, MAX); 
		n = 0; 
	}
	printf("exited for loop in receiveFunc on client");
}

void sendSignalToTurn(HRAC *hrac, char buff[])
{
	if (strncmp("w", buff,1)==0) 
	{
		if (hrac->HADIK->snakeDirectionY != 1 || hrac->HADIK->dlzka == 1) // ak nejde dole, moze ist hore
		{
			hrac->HADIK->snakeDirectionX = 0;
			hrac->HADIK->snakeDirectionY = -1;
			//printf(buff);
		}
		else
		{
			//SDL_Log("Illegal move!");
			// printf("Illegal move!");
		}
	} else if (strncmp("s", buff,1)==0)
	{
		if (hrac1->HADIK->snakeDirectionY != -1 || hrac1->HADIK->dlzka == 1) // ak nejde dole, moze ist hore
		{
			hrac->HADIK->snakeDirectionX = 0;
			hrac->HADIK->snakeDirectionY = 1;
			//printf(buff);
		}
		else
		{
			//SDL_Log("Illegal move!");
			// printf("Illegal move!");
		}
	} else if (strncmp("a", buff,1)==0)
	{
		if (hrac1->HADIK->snakeDirectionX != 1 || hrac1->HADIK->dlzka == 1) // ak nejde dole, moze ist hore
		{
			hrac->HADIK->snakeDirectionX = -1;
			hrac->HADIK->snakeDirectionY = 0;
			//printf(buff);
		}
		else
		{
			//SDL_Log("Illegal move!");
			// printf("Illegal move!");
		}
	} else if (strncmp("d", buff,1)==0)
	{
		if (hrac1->HADIK->snakeDirectionX != -1 || hrac1->HADIK->dlzka == 1) // ak nejde dole, moze ist hore
		{
			hrac->HADIK->snakeDirectionX = 1;
			hrac->HADIK->snakeDirectionY = 0;
			//printf(buff);
		}
		else
		{
			//SDL_Log("Illegal move!");
			// printf("Illegal move!");
		}
	}else {
        printf("Unknown command: %s\n", buff);
    }
}

void* sendThreadFunc(void* arg) {
	struct sendArguments *localArgs = (struct sendArguments *)arg;
	sendFunc((*localArgs->sockfd),*(localArgs->event)); 
	return NULL;
}

void* receiveThreadFunc(void* arg) {
	int sockfd = *(int*)arg;
	receiveFunc(sockfd);
	return NULL;
}

void *playerThreadFunc(void *arg)
{
	//HRAC* hrac = (HRAC*)arg;
	// struct arguments *localArgs = (struct arguments *)arg;
	//SDL_Event *event = (SDL_Event *)arg;
	// pthread_mutex_lock(&mutex);
	gameLoop(hrac1, hrac2);
	// pthread_mutex_unlock(&mutex);
	//  cleanupSDL();
	return NULL;
}

void *vykreslovacieVlaknoFunc(void *arg)
{
	while (!quitGraphics)
	{
		renderCellH1(hrac1); // viem, ze to nie je clean code ale co uz
		renderCellH2(hrac2);

		if (hrac1->hracVyhral == 1)
		{
			SDL_Log(" %d vykreslovacieVlaknoFunc - game over hrac1->hracVyhral == 1", hrac1->hracVyhral);
			gameOver(hrac1);
			hrac1->hracVyhral = 5;
		}
		else if (hrac2->hracVyhral == 1)
		{
			SDL_Log("vykreslovacieVlaknoFunc - game over: hrac2->hracVyhral == 1", hrac2->hracVyhral);
			gameOver(hrac2);
			hrac2->hracVyhral = 5;
		}
	}
	return NULL;
}

int main()
{
	
	

	pthread_t sendThread, receiveThread;

	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;


	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(PORT);

	// connect the client socket to server socket
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr))
		!= 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	//SDL_Window* window;
	//window = SDL_CreateWindow("Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);
    //SDL_Init(SDL_INIT_VIDEO);
	//SDL_Init(SDL_INIT_EVENTS);
	
	hrac1 = vytvorHraca("Kade", 5);
	hrac1->HADIK->x = 1;
	hrac1->HADIK->y = 1;
	hrac1->HADIK->snakeDirectionX = 1; // Initial direction (right)
	hrac1->HADIK->snakeDirectionY = 0;
	hrac1->HADIK->previousTailX = 0;
	hrac1->HADIK->previousTailY = 0;
	hrac1->hracVyhral= 0;

	hrac1->snake_image_surface = IMG_Load("images/snake_pattern.jpg");
	if (!(hrac1->snake_image_surface))
	{
		SDL_Log("Failed to initialize Snake skin surface - SDL_image: %s\n", IMG_GetError());
	}

	hrac2 = vytvorHraca("Marek", 5);
	hrac2->HADIK->x = 1;
	hrac2->HADIK->y = 3;
	hrac2->HADIK->snakeDirectionX = 1;
	hrac2->HADIK->snakeDirectionY = 0;
	hrac2->HADIK->previousTailX = 0;
	hrac2->HADIK->previousTailY = 0;
	hrac2->hracVyhral= 0;
	hrac2->snake_image_surface = IMG_Load("images/snake2_pattern.png");
	
	if (!(hrac1->snake_image_surface))
	{
		SDL_Log("Failed to initialize Snake skin surface - SDL_image: %s\n", IMG_GetError());
	}

	pthread_t vlaknoHrac1, vykreslovacieVlakno;


	initGame(hrac1, hrac2);
	initSnake(hrac1, hrac2);
	randomFood();
	drawGameBoard();
	// gameOver(hrac2);
	pthread_create(&vlaknoHrac1, NULL, playerThreadFunc, NULL);

	pthread_create(&vykreslovacieVlakno, NULL, vykreslovacieVlaknoFunc, NULL);
	// function for chat
	//receiveFunc(sockfd);
	SDL_Event event;
	struct sendArguments *sendArgs = malloc(sizeof(struct sendArguments));
	sendArgs->sockfd=&sockfd;
	sendArgs->event=&event;
	
	pthread_create(&sendThread, NULL, sendThreadFunc, sendArgs);
	pthread_create(&receiveThread, NULL, receiveThreadFunc, &sockfd);

	pthread_join(receiveThread, NULL);
	pthread_join(vlaknoHrac1, NULL);
	pthread_join(vykreslovacieVlakno, NULL);
	pthread_join(sendThread, NULL);
	// close the socket

	close(sockfd);
	cleanupSDL();
	free(sendArgs);
	destroyHrac(hrac1);
	destroyHrac(hrac2);
	return 0;
}
