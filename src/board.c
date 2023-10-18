#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10
#define NUM_SHIPS 5
#define HORIZONTAL 'H'
#define VERTICAL 'V'
#define EMPTY 'A'
#define SHIP 'B'
#define HIT 'X'
#define MISS 'O'

// Estructura para un barco
typedef struct
{
    int size;
    char orientation; // 'H' para horizontal, 'V' para vertical
    int x, y;         // Coordenadas del extremo superior izquierdo del barco
} Ship;

// Estructura para el tablero
typedef struct
{
    char grid[BOARD_SIZE][BOARD_SIZE]; // Matriz de caracteres para representar el tablero
} Board;

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

int main()
{
    srand(time(NULL));
    Board playerBoard;
    initializeBoard(&playerBoard);

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

            placed = placeShip(&playerBoard, ship);
        }
    }

    printf("Tablero del Jugador:\n");
    printBoard(&playerBoard);

    return 0;
}
