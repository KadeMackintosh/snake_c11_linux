#include <stdio.h>
#include <SDL.h>
#include <pthread.h>
#include <SFML/Network.h>
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/select.h>
#include <unistd.h> // read(), write(), close()

#include "hrac.h"
#include "hracie_pole.h"

#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
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

struct sendArguments{
	int *connfd;
	SDL_Event *event;
};

void sendFunc(int connfd, SDL_Event event)
{
	char buff[MAX];

    for (;;) {
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
                    default:
                        // Ignore other keys
                        continue;  // Skip the rest of the loop for non-matching keys
                }

                // Send the message to the server immediately after key press
                write(connfd, buff, sizeof(buff));
				if (strncmp(buff, "q", 1) == 0)
				{
					printf("Exiting server by server...");
					cleanupSDL();
					break;
			}
				sendSignalToTurn(hrac1, buff);
            }
        }
    }
}

void receiveFunc(int connfd) 
{ 
	char buff[MAX]; 
	int n; 
	// infinite loop for chat 
	for (;;) { 
		bzero(buff, MAX);
		// read the message from client and copy it in buffer 
		read(connfd, buff, sizeof(buff)); 
		// print buffer which contains the client contents 
		printf("From client: %s\n", buff);
		sendSignalToTurn(hrac2, buff);
		if (strncmp(buff, "q", 1) == 0)
		{
			printf("Exiting server by client...");
			cleanupSDL();
			break;
		}
		SDL_Delay(80);
		bzero(buff, MAX); 
		n = 0; 
	} 
}

void* sendThreadFunc(void* arg) {
	struct sendArguments *localArgs = (struct sendArguments *)arg;
	sendFunc((*localArgs->connfd),*(localArgs->event)); 
	return NULL;
}

void* receiveThreadFunc(void* arg) {
	int connfd = *(int*)arg;
	receiveFunc(connfd);
	return NULL;
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

void *playerThreadFunc(void *arg)
{
	gameLoop(hrac1, hrac2);
	return NULL;
}

void *vykreslovacieVlaknoFunc(void *arg)
{
	while (1)
	{
		renderCellH1(hrac1); // viem, ze to nie je clean code ale co uz
		renderCellH2(hrac2);
	}

	return NULL;
}

int main()
{	
	int sockfd, connfd, len; 
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
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server accept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server accept the client...\n"); 
	// ------------------------------------
	pthread_t vykreslovacieVlakno, vlaknoHraci;

	hrac1 = vytvorHraca("Kade", 5);
	hrac1->HADIK->x = 1;
	hrac1->HADIK->y = 1;
	hrac1->HADIK->snakeDirectionX = 1; // Initial direction (right)
	hrac1->HADIK->snakeDirectionY = 0;
	hrac1->HADIK->previousTailX = 0;
	hrac1->HADIK->previousTailY = 0;

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

	hrac2->snake_image_surface = IMG_Load("images/snake2_pattern.png");
	
	if (!(hrac1->snake_image_surface))
	{
		SDL_Log("Failed to initialize Snake skin surface - SDL_image: %s\n", IMG_GetError());
	}

	initGame(hrac1, hrac2);
	initSnake(hrac1, hrac2);
	randomFood();
	drawGameBoard(hrac1, hrac2);
	pthread_create(&vlaknoHraci, NULL, playerThreadFunc, NULL);
	pthread_create(&vykreslovacieVlakno, NULL, vykreslovacieVlaknoFunc, NULL);

	SDL_Event event;
	struct sendArguments *sendArgs = malloc(sizeof(struct sendArguments));
	sendArgs->connfd=&connfd;
	sendArgs->event=&event;


	pthread_t receiveThread, sendThread;
	pthread_create(&receiveThread, NULL, receiveThreadFunc, &connfd);
	pthread_create(&sendThread, NULL, sendThreadFunc, sendArgs);
	pthread_join(sendThread, NULL);
	pthread_join(receiveThread, NULL);
	pthread_join(vlaknoHraci, NULL);
	pthread_join(vykreslovacieVlakno, NULL);
	
	free(sendArgs);
	destroyHrac(hrac1);
	destroyHrac(hrac2);
	return 0;
}
