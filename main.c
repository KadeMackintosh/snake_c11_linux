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

void sendFunc(int sockfd)
{
	char buff[MAX];
	int n;
	for (;;) {
		bzero(buff, sizeof(buff));
		n = 0;
		while ((buff[n++] = getchar()) != '\n')
			;
		write(sockfd, buff, sizeof(buff));
	}
}

// void sendFunc(int sockfd) {
//     char buff[MAX];
//     int n;

//     for (;;) {
//         bzero(buff, sizeof(buff));

//         // Check if input is available
//         fd_set rfds;
//         struct timeval tv;
//         FD_ZERO(&rfds);
//         FD_SET(STDIN_FILENO, &rfds);
//         tv.tv_sec = 0;
//         tv.tv_usec = 0;

//         // Wait for a short time to check if input is available
//         if (select(STDIN_FILENO + 1, &rfds, NULL, NULL, &tv) > 0) {
//             // Input is available, read it
//             n = 0;
//             while ((buff[n++] = getchar()) != '\n');
//             write(sockfd, buff, sizeof(buff));
//         }

//         // Add some delay to avoid high CPU usage
//         usleep(10000);  // Sleep for 10 milliseconds
//     }
// }


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
	int connfd = *(int*)arg;
	sendFunc(connfd);
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
			printf(buff);
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
			printf(buff);
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
			printf(buff);
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
			printf(buff);
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
	SDL_Event *event = (SDL_Event *)arg;
	// pthread_mutex_lock(&mutex);
	gameLoop(hrac1, NULL, event);
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

	SDL_Event event1;
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
	pthread_create(&vlaknoHrac1, NULL, playerThreadFunc, &event1);
	// // pthread_create(&vlaknoHrac2, NULL, playerThreadFunc, hrac2Args);
	pthread_create(&vykreslovacieVlakno, NULL, vykreslovacieVlaknoFunc, NULL);
	// // gameLoop(hrac2);
	// // initMenu();

	// // menuLoop(hrac1);
	// // cleanUpMenu();

	
	// // pthread_join(vlaknoHrac2, NULL);
	// pthread_join(vykreslovacieVlakno, NULL);
	// // pthread_mutex_destroy(&mutex);

	pthread_t receiveThread, sendThread;
	pthread_create(&receiveThread, NULL, receiveFunc, connfd);
	// pthread_create(&sendThread, NULL, sendThreadFunc, connfd);
	pthread_join(receiveThread, NULL);
	pthread_join(vlaknoHrac1, NULL);
	pthread_join(vykreslovacieVlakno, NULL);
	return 0;
}
