#include "../board.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    Board playerBoard;

    generateBoard(&playerBoard);

    printf("Tablero del Jugador:\n");
    printBoard(&playerBoard);

    return 0;
}