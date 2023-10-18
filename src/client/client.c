#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));
    Board playerBoard;
    initializeBoard(&playerBoard);

    // Aquí puedes crear e inicializar los barcos y colocarlos en el tablero

    printf("Tablero del Jugador:\n");
    printBoard(&playerBoard);

    // Puedes continuar con la lógica del juego aquí

    return 0;
}
