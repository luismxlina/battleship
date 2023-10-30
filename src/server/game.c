// game.c

#include "game.h"
#include "board.h"
#include <stdbool.h>
#include <stdio.h>

// Función para realizar un disparo en el tablero del oponente
int makeShot(Board *opponentBoard, int x, int y)
{
    // Verifica si las coordenadas están dentro del tablero
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
    {
        return -1; // Devuelve -1 si las coordenadas están fuera del tablero
    }

    // Verifica si la casilla ya ha sido disparada
    if (opponentBoard->grid[y][x] == HIT || opponentBoard->grid[y][x] == MISS)
    {
        return 0; // Devuelve 0 si ya se hizo un disparo en esa casilla
    }

    // Realiza el disparo
    if (opponentBoard->grid[y][x] == SHIP)
    {
        opponentBoard->grid[y][x] = HIT; // Marcamos como un acierto
        return 1;                        // Devuelve 1 si se acertó un barco
    }
    else
    {
        opponentBoard->grid[y][x] = MISS; // Marcamos como un fallo
        return 2;                         // Devuelve 2 si se disparó al agua
    }
}

// Función para verificar si un jugador ha ganado la partida
bool hasPlayerWon(Board *opponentBoard)
{
    // Recorre el tablero del oponente para verificar si todos los barcos han sido hundidos
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (opponentBoard->grid[i][j] == SHIP)
            {
                return false; // Si se encuentra un barco no hundido, el jugador no ha ganado
            }
        }
    }
    return true; // Si no se encontraron barcos en el tablero del oponente, el jugador ha ganado
}

void initializeGame(Player *player1, Player *player2, char *msg1, char *msg2)
{
    Game *game = (Game *)malloc(sizeof(Game)); // Se crea la estructura del juego
    generateBoard(&game->board1);              // Se genera el tablero del jugador 1 (el que inicia)
    generateBoard(&game->board2);              // Se genera el tablero del jugador 2 (el que responde)

    bzero(msg1, sizeof(msg1));
    bzero(msg2, sizeof(msg2));

    boardToString(&game->board1, msg1); // Se convierte el tablero del jugador 1 a una cadena
    boardToString(&game->board2, msg2); // Se convierte el tablero del jugador 2 a una cadena

    player1->status = 4;  // Se cambia el estado del jugador 1 a jugando
    player1->game = game; // Se asigna el juego al jugador 1

    player2->status = 4;
    player2->game = game;

    game->player1 = player1;
    game->player2 = player2;
    game->turn = 1;
}
