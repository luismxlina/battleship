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
Player *initializePlayer(int socket)
{
    Player *newPlayer = (Player *)malloc(sizeof(Player));
    newPlayer->name = (char *)malloc(sizeof(char) * 50);
    newPlayer->socket = socket;
    newPlayer->status = 0;
    newPlayer->game = NULL;
    return newPlayer;
}

char *readfile(const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long fileSize = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *contenido = (char *)malloc(fileSize + 1);

    if (contenido == NULL)
    {
        fclose(archivo);
        return NULL;
    }

    size_t bytesRead = fread(contenido, 1, fileSize, archivo);
    contenido[bytesRead] = '\0';

    fclose(archivo);
    return contenido;
}

void exitClient(Player *player, fd_set *readfds, int *numClientes, List **list)
{
    int socket = player->socket;
    char buffer[MSG_SIZE];
    if (player->status == 4)
    {
        Player *rival;
        if (player->game->player1->socket != socket)
        {
            rival = player->game->player1;
        }
        else
        {
            rival = player->game->player2;
        }
        bzero(buffer, sizeof(buffer));
        strcpy(buffer, "+Ok. Tu rival ha salido de la partida.\n");
        send(rival->socket, buffer, sizeof(buffer), 0);

        rival->status = 2;
        free(rival->game);
        rival->game = NULL;
    }
    removePlayer(list, socket);
    close(socket);
    FD_CLR(socket, readfds);
    (*numClientes)--;
    bzero(buffer, sizeof(buffer));
    sprintf(buffer, "+Ok. Desconexión del cliente: %d\n", socket);
}

void signalHandler(int signum)
{
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT, signalHandler);

    // Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}