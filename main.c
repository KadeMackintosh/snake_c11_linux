#include <stdio.h>
#include <SDL.h>
#include <pthread.h>
#include "hracie_pole.h"
#include <SFML/Network.h>
#include "hrac.h"
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/select.h>
#include <unistd.h>

#include <unistd.h> // read(), write(), close()
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
		sendSignalToTurn(hrac1, buff);
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
	if (strncmp("8", buff,1)==0) 
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
	} else if (strncmp("5", buff,1)==0)
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
	} else if (strncmp("4", buff,1)==0)
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
	} else if (strncmp("6", buff,1)==0)
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
	// struct arguments *localArgs = (struct arguments *)arg;
	// pthread_mutex_lock(&mutex);
	gameLoop(hrac1, NULL, NULL);
	// pthread_mutex_unlock(&mutex);
	//  cleanupSDL();
	return NULL;
}

void *vykreslovacieVlaknoFunc(void *arg)
{
	while (1)
	{
		renderCell(hrac1);
		//renderCell(hrac2);
	}

	// HRAC* hrac = (HRAC*)arg;

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
	
	pthread_t vykreslovacieVlakno, vlaknoHrac1, vlaknoHrac2;

	// pthread_mutex_init(&mutex, NULL);

	hrac1 = vytvorHraca("Kade", 5);
	hrac1->HADIK->x = 1;
	hrac1->HADIK->y = 1;
	hrac1->HADIK->snakeDirectionX = 1; // Initial direction (right)
	hrac1->HADIK->snakeDirectionY = 0;
	hrac1->HADIK->previousTailX = 0;
	hrac1->HADIK->previousTailY = 0;

	// hrac2 = vytvorHraca("Marek", 5);
	// hrac2->HADIK->x = 1;
	// hrac2->HADIK->y = 3;
	// hrac2->HADIK->snakeDirectionX = 1;
	// hrac2->HADIK->snakeDirectionY = 0;
	// hrac2->HADIK->previousTailX = 0;
	// hrac2->HADIK->previousTailY = 0;



	//SDL_Event event1;
	// // struct arguments *hrac1Args = malloc(sizeof(struct arguments));
	// // hrac1Args->event = &event1;
	// // hrac1Args->hrac = hrac1;
	// // hrac1Args->arrow_control = SDL_TRUE;

	// // SDL_Event event2;
	// // struct arguments* hrac2Args = malloc(sizeof(struct arguments));
	// // hrac2Args->event = &event2;
	// // hrac2Args->hrac = hrac2;
	// // hrac1Args->arrow_control = SDL_FALSE;
	initGame();
	initSnake(hrac1);
	// initSnake(hrac2);
	randomFood();
	drawGameBoard();
	// pthread_create(&serverThreadId, NULL, serverThreadFunc, NULL);
	pthread_create(&vlaknoHrac1, NULL, playerThreadFunc, NULL);
	// // pthread_create(&vlaknoHrac2, NULL, playerThreadFunc, hrac2Args);
	pthread_create(&vykreslovacieVlakno, NULL, vykreslovacieVlaknoFunc, NULL);
	// // gameLoop(hrac2);
	// // initMenu();

	// // menuLoop(hrac1);
	// // cleanUpMenu();
	SDL_Event event;
	struct sendArguments *sendArgs = malloc(sizeof(struct sendArguments));
	sendArgs->connfd=&connfd;
	sendArgs->event=&event;
	
	// // pthread_join(vlaknoHrac2, NULL);
	// pthread_join(vykreslovacieVlakno, NULL);
	// // pthread_mutex_destroy(&mutex);

	pthread_t receiveThread, sendThread;
	pthread_create(&receiveThread, NULL, receiveThreadFunc, &connfd);
	pthread_create(&sendThread, NULL, sendThreadFunc, sendArgs);
	pthread_join(sendThread, NULL);
	pthread_join(receiveThread, NULL);
	pthread_join(vlaknoHrac1, NULL);
	pthread_join(vykreslovacieVlakno, NULL);
	return 0;
}
