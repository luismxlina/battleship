#ifndef SERVERUTILS_H
#define SERVERUTILS_H
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define PORT 2000
#define MSG_SIZE 1000
#define MAX_CLIENTS 50

// Estructura para representar un cliente
// typedef struct
// {
//     int socket;
//     struct sockaddr_in address;
// } Client;

// Client initializeClient(int socket);
// char *readfile(const char *nombreArchivo);
void exitClient(int socket, fd_set *readfds, int *numClientes, int arrayClientes[]);
void signalHandler(int signum);
#endif