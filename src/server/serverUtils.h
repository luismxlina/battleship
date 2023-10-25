#ifndef SERVERUTILS_H
#define SERVERUTILS_H
#include <stdlib.h>
#define PORT 2000
#define MSG_SIZE 1000

// Estructura para representar un cliente
typedef struct
{
    int socket;
    struct sockaddr_in address;
} Client;

Client initializeClient(int socket);

    char *readfile(const char *nombreArchivo);
#endif