#include "serverUtils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

// Función para inicializar un nuevo cliente
Client *initializeClient(int socket)
{
    Client *newClient;
    newClient.socket = socket;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    getpeername(socket, (struct sockaddr *)&newClient.address, &addrSize);
    return newClient;
}

// char *readfile(const char *nombreArchivo)
// {
//     FILE *archivo = fopen(nombreArchivo, "r");

//     if (archivo == NULL)
//     {
//         printf("No se pudo abrir el archivo %s\n", nombreArchivo);
//         return NULL;
//     }

//     fseek(archivo, 0, SEEK_END);
//     long fileSize = ftell(archivo);
//     fseek(archivo, 0, SEEK_SET);

//     char *contenido = (char *)malloc(fileSize + 1);

//     if (contenido == NULL)
//     {
//         fclose(archivo);
//         return NULL;
//     }

//     size_t bytesRead = fread(contenido, 1, fileSize, archivo);
//     contenido[bytesRead] = '\0';

//     fclose(archivo);
//     return contenido;
// }

void exitClient(int socket, fd_set *readfds, int *numClientes, Client arrayClientes[])
{

    char buffer[250];
    int j;

    close(socket);
    FD_CLR(socket, readfds);

    // Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j].socket == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j + 1]);

    (*numClientes)--;

    bzero(buffer, sizeof(buffer));
    sprintf(buffer, "Desconexión del cliente <%d>", socket);

    for (j = 0; j < (*numClientes); j++)
        if (arrayClientes[j].socket != socket)
            send(arrayClientes[j].socket, buffer, sizeof(buffer), 0);
}

void signalHandler(int signum)
{
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT, signalHandler);

    // Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}