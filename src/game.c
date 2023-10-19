// game.c

#include "game.h"
#include "board.h"
#include <stdbool.h>
#include <stdio.h>

// Función para realizar un disparo en el tablero del oponente
bool makeShot(Board *opponentBoard, int x, int y)
{
    // Verifica si las coordenadas están dentro del tablero
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
    {
        return false; // Devuelve falso si las coordenadas están fuera del tablero
    }

    // Verifica si la casilla ya ha sido disparada
    if (opponentBoard->grid[y][x] == HIT || opponentBoard->grid[y][x] == MISS)
    {
        return false; // Devuelve falso si ya se hizo un disparo en esa casilla
    }

    // Realiza el disparo
    if (opponentBoard->grid[y][x] == SHIP)
    {
        opponentBoard->grid[y][x] = HIT; // Marcamos como un acierto
        return true;                     // Devuelve verdadero si se acertó un barco
    }
    else
    {
        opponentBoard->grid[y][x] = MISS; // Marcamos como un fallo
        return false;                     // Devuelve falso si se disparó al agua
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