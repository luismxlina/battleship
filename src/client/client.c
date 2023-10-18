#include "../board.h"
#include "../auth.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    Board playerBoard;

    auth();

    generateBoard(&playerBoard);

    printf("Tablero del Jugador:\n");
    printBoard(&playerBoard);

    char boardString[BOARD_SIZE * BOARD_SIZE + 1]; // +1 para el carácter nulo
    boardToString(&playerBoard, boardString);

    printf("\nTablero en forma de cadena:\n");
    printf("%s\n", boardString);

    return 0;
}