#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE 10
#define HORIZONTAL 'H'
#define VERTICAL 'V'
#define EMPTY ' '
#define SHIP 'S'
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

void placeShip(Board *board, Ship ship)
{
    // Verifica que el barco quepa en el tablero en la posición y orientación especificadas
    if (ship.x < 0 || ship.x >= BOARD_SIZE || ship.y < 0 || ship.y >= BOARD_SIZE)
    {
        printf("Posición fuera del tablero. No se puede colocar el barco.\n");
        return;
    }

    // Verifica que no haya otros barcos en las casillas donde se coloca el barco
    for (int i = 0; i < ship.size; i++)
    {
        if (ship.orientation == HORIZONTAL)
        {
            if (board->grid[ship.y][ship.x + i] == SHIP)
            {
                printf("Barco en conflicto con otro barco. No se puede colocar.\n");
                return;
            }
        }
        else
        {
            if (board->grid[ship.y + i][ship.x] == SHIP)
            {
                printf("Barco en conflicto con otro barco. No se puede colocar.\n");
                return;
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

int main()
{
    srand(time(NULL));
    Board playerBoard;
    initializeBoard(&playerBoard);

    // Aquí puedes crear e inicializar los barcos y colocarlos en el tablero
    Ship ship1 = {3, HORIZONTAL, 0, 0};
    Ship ship2 = {4, VERTICAL, 5, 5};
    Ship ship3 = {5, HORIZONTAL, 7, 2};
    placeShip(&playerBoard, ship1);
    placeShip(&playerBoard, ship2);
    placeShip(&playerBoard, ship3);

    printf("Tablero del Jugador:\n");
    printBoard(&playerBoard);

    // Puedes continuar con la lógica del juego aquí

    return 0;
}
