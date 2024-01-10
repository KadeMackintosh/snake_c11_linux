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
#include "hracie_pole.h"
#include "hrac.h"
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
// void sendFunc(int sockfd)
// {
// 	char buff[MAX];
// 	int n;
// 	for (;;) {
// 		bzero(buff, sizeof(buff));
// 		n = 0;
// 		while ((buff[n++] = getchar()) != '\n')
// 			;
// 		write(sockfd, buff, sizeof(buff));
// 	}
// }

struct sendArguments{
	int *sockfd;
	SDL_Event *event;
};

HRAC *hrac1;

void sendFunc(int sockfd, SDL_Event event) {
	

    // Create an SDL window
    
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
                write(sockfd, buff, sizeof(buff));
            }
        }
    }
}


// void receiveFunc(int connfd) 
// { 
// 	char buff[MAX]; 
// 	int n; 
// 	// infinite loop for chat 
// 	for (;;) { 
// 		bzero(buff, MAX);
// 		// read the message from client and copy it in buffer 
// 		read(connfd, buff, sizeof(buff)); 
// 		// print buffer which contains the client contents 
// 		printf("From client: %s\n", buff);
// 		sendSignalToTurn(hrac1, buff);
// 		SDL_Delay(80);
// 		bzero(buff, MAX); 
// 		n = 0; 
// 	} 
// }

void receiveFunc(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		read(sockfd, buff, sizeof(buff));
		printf("From Server : %s\n", buff);
		sendSignalToTurn(hrac1, buff);
		SDL_Delay(80);
		bzero(buff, MAX); 
		n = 0; 
	}
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
	// struct arguments *localArgs = (struct arguments *)arg;
	//SDL_Event *event = (SDL_Event *)arg;
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
	
	pthread_t vlaknoHrac1, vykreslovacieVlakno;


	initGame();
	initSnake(hrac1);
	// initSnake(hrac2);
	randomFood();
	drawGameBoard();
	pthread_create(&vlaknoHrac1, NULL, playerThreadFunc, NULL);
	pthread_create(&vykreslovacieVlakno, NULL, vykreslovacieVlaknoFunc, NULL);
	// function for chat
	//receiveFunc(sockfd);
	SDL_Event event;
	struct sendArguments *sendArgs = malloc(sizeof(struct sendArguments));
	sendArgs->sockfd=&sockfd;
	sendArgs->event=&event;
	
	//pthread_create(&sendThread, NULL, sendThreadFunc, sendArgs);
	pthread_create(&receiveThread, NULL, receiveThreadFunc, &sockfd);

	pthread_join(receiveThread, NULL);
	pthread_join(vlaknoHrac1, NULL);
	pthread_join(vykreslovacieVlakno, NULL);
	//pthread_join(sendThread, NULL);
	// close the socket
	close(sockfd);

	
}
