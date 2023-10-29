// Descripción: Funciones para manejar el tablero de juego.
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Inicializa el tablero con celdas vacías
void initializeBoard(Board *board)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board->grid[i][j] = EMPTY;
        }
    }
}

bool placeShip(Board *board, Ship ship)
{
    // Verifica que el barco quepa en la posición y orientación especificadas
    if (ship.x < 0 || ship.x >= BOARD_SIZE || ship.y < 0 || ship.y >= BOARD_SIZE)
    {
        return false; // Devuelve falso si la posición está fuera del tablero
    }

    // Verifica si hay otros barcos ocupando las casillas donde se coloca el barco
    for (int i = 0; i < ship.size; i++)
    {
        if (ship.orientation == HORIZONTAL)
        {
            if (board->grid[ship.y][ship.x + i] == SHIP)
            {
                return false; // Devuelve falso si hay conflicto con otro barco
            }
        }
        else
        {
            if (board->grid[ship.y + i][ship.x] == SHIP)
            {
                return false; // Devuelve falso si hay conflicto con otro barco
            }
        }
    }

    // Verifica si hay barcos adyacentes
    for (int i = ship.y - 1; i <= ship.y + ship.size; i++)
    {
        for (int j = ship.x - 1; j <= ship.x + ship.size; j++)
        {
            if (i >= 0 && i < BOARD_SIZE && j >= 0 && j < BOARD_SIZE)
            {
                if (board->grid[i][j] == SHIP)
                {
                    return false; // Devuelve falso si hay un barco adyacente
                }
            }
        }
    }

    // Coloca el barco en el tablero
    for (int i = 0; i < ship.size; i++)
    {
        if (ship.orientation == HORIZONTAL)
        {
            board->grid[ship.y][ship.x + i] = SHIP;
        }
        else
        {
            board->grid[ship.y + i][ship.x] = SHIP;
        }
    }

    return true; // Devuelve true si el barco se colocó con éxito
}

// Imprime el tablero
void printBoard(Board *board)
{
    printf(" ");
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf(" %c", 'a' + i);
    }
    printf("\n");

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        printf("%d ", i);
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            printf("%c ", board->grid[i][j]);
        }
        printf("\n");
    }
}

// Función para generar un valor pseudoaleatorio entre min y max (ambos inclusive)
int getRandom(int min, int max)
{
    return rand() % (max - min + 1) + min;
}

void generateBoard(Board *board)
{
    initializeBoard(board);

    // Número y tamaño de los barcos
    int shipSizes[] = {4, 3, 3, 2, 2};

    // Genera y coloca barcos pseudoaleatoriamente
    for (int i = 0; i < NUM_SHIPS; i++)
    {
        Ship ship;
        ship.size = shipSizes[i];
        ship.orientation = (getRandom(0, 1) == 0) ? HORIZONTAL : VERTICAL; // 0 para horizontal, 1 para vertical

        bool placed = false;
        while (!placed)
        {
            if (ship.orientation == HORIZONTAL)
            {
                ship.x = getRandom(0, BOARD_SIZE - ship.size);
                ship.y = getRandom(0, BOARD_SIZE - 1);
            }
            else
            {
                ship.x = getRandom(0, BOARD_SIZE - 1);
                ship.y = getRandom(0, BOARD_SIZE - ship.size);
            }

            placed = placeShip(board, ship);
        }
    }
}

void boardToString(Board *board, char *output)
{
    int index = 0;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            output[index] = board->grid[i][j];
            index++;
        }
    }
    output[index] = '\0'; // Agrega un carácter nulo al final de la cadena
}

int stringToBoard(Board *board, char *str)
{
    // Comprobar si la cadena es válida
    if (board == NULL || str == NULL || strlen(str) != BOARD_SIZE * BOARD_SIZE)
    {
        return -1;
    }

    // Convertir la cadena en un tablero de juego
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (str[i * BOARD_SIZE + j] == EMPTY)
            {
                board->grid[i][j] = EMPTY;
            }
            else if (str[i * BOARD_SIZE + j] == SHIP)
            {
                board->grid[i][j] = SHIP;
            }
            else if (str[i * BOARD_SIZE + j] == HIT)
            {
                board->grid[i][j] = HIT;
            }
            else if (str[i * BOARD_SIZE + j] == MISS)
            {
                board->grid[i][j] = MISS;
            }
            else
            {
                return -1;
            }
        }
    }

    return 0;
}