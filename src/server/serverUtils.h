#ifndef SERVERUTILS_H
#define SERVERUTILS_H
#include "board.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#define PORT 2000
#define MSG_SIZE 1000
#define MAX_CLIENTS 50
#define MAX_GAMES
#define FILENAME "usuarios.dat"

// Estructura para representar un cliente
typedef struct player
{
    int socket;
    char *name;
    int status; /* 0: conectado no identificado
                 * 1: registrado, esperando password
                 * 2: registrado, esperando iniciar partida
                 * 3: en espera de match
                 * 4: jugando
                 * */

    Game *game;
} Player;

typedef struct game
{
    Board board;
    Player *player1;
    Player *player2;
    int turn;
} Game;

typedef struct list
{
    Player *item;
    List *next;
} List;

Player *initializePlayer(int socket);
void exitClient(Player *player, fd_set *readfds, int *numClientes, List **list);
void signalHandler(int signum);
char *readfile(const char *nombreArchivo);
#endif