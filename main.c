#include <stdio.h>
#include <SDL.h>
#include <pthread.h>
#include "hracie_pole.h"
#include "hrac.h"

#include <SFML/Network.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345


// pthread_mutex_t boardMutex = PTHREAD_MUTEX_INITIALIZER;
// enum CellType gameBoard[BOARD_SIZE_X][BOARD_SIZE_Y];

// struct arguments
// {
// 	HRAC *hrac;
// 	SDL_Event *event;
// 	void (*loopFunction)(HRAC *, SDL_Event *, enum CellType (*)[BOARD_SIZE_X][BOARD_SIZE_Y]);
// };

// void *playerFunc(void *arg){
//     pthread_mutex_lock(&boardMutex);
// 	struct arguments *localArgs = (struct arguments *)arg;
// 	localArgs->loopFunction(localArgs->hrac, localArgs->event, &gameBoard);
//     pthread_mutex_unlock(&boardMutex);
// 	return NULL;
// }

// void *renderFunction(void *arg){
// 	struct arguments *localArgs = (struct arguments *)arg;
// 	// pthread_mutex_lock(&boardMutex);
// 	updateSnakePosition(localArgs->hrac, &gameBoard);
//     // pthread_mutex_unlock(&boardMutex);
// 	return NULL;
// }

int main()
{

	//    // Create a TCP socket
    // sfTcpListener* listener = sfTcpListener_create();

    // // Bind the listener to a port
    // if (sfTcpListener_listen(listener, PORT, sfIpAddress_Any) != sfSocketDone) {
    //     // Handle error
    //     fprintf(stderr, "Error listening on port %d\n", PORT);
    //     return 1;
    // }

    // printf("Server is listening...\n");

    // // Accept a new connection
    // sfTcpSocket* client;
    // sfSocketStatus status = sfTcpListener_accept(listener, &client);

    // if (status != sfSocketDone) {
    //     // Handle error
    //     fprintf(stderr, "Error accepting connection\n");
    //     return 1;
    // }

    // // Send a message to the client
    // const char* message = "Hello, client!";
    // sfTcpSocket_send(client, message, strlen(message) + 1);

    // // Receive a message from the client
    // char buffer[100];
    // size_t received;
    // sfTcpSocket_receive(client, buffer, sizeof(buffer), &received);
    // printf("Received from client: %s\n", buffer);

    // // Close the client socket
    // sfTcpSocket_destroy(client);

    // // Close the listener socket
    // sfTcpListener_destroy(listener);
	//-------------------------------------
	// pthread_t vykreslovacieVlakno, hracskeVlakno1;
 	// pthread_mutex_init(&boardMutex, NULL);
	SDL_Event event1;
	HRAC *hrac1 = vytvorHraca("Kade", 1);
	hrac1->HADIK->x = 1;
	hrac1->HADIK->y = 1;
	hrac1->HADIK->snakeDirectionX = 1; // Initial direction (right)
	hrac1->HADIK->snakeDirectionY = 0;


	// struct arguments* hrac1Args = malloc(sizeof(struct arguments));
	// hrac1Args->event = &event1;
	// hrac1Args->hrac = hrac1;
	// hrac1Args->loopFunction = gameLoop;

	// SDL_Event event1;
	// struct arguments *renderArgs = malloc(sizeof(struct arguments));
    // renderArgs->event = &event1;
    // renderArgs->hrac = hrac1;

	initGame(&gameBoard);
	initSnake(hrac1, &gameBoard);
	randomFood(&gameBoard);
	drawGameBoard(&gameBoard);

	gameLoop(hrac1, &event1, &gameBoard);
	// pthread_create(&hracskeVlakno1, NULL, playerFunc, hrac1Args);
	// pthread_create(&vykreslovacieVlakno, NULL, renderFunction, renderArgs);

	// pthread_join(hracskeVlakno1, NULL);
	// pthread_join(vykreslovacieVlakno, NULL);

	return 0;
}
