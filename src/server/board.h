#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#define BOARD_SIZE 10
#define NUM_SHIPS 5
#define HORIZONTAL 'H'
#define VERTICAL 'V'
#define EMPTY 'A'
#define SHIP 'B'
#define HIT 'X'
#define MISS 'O'

typedef struct
{
    int size;
    char orientation;
    int x, y;
} Ship;

typedef struct
{
    char grid[BOARD_SIZE][BOARD_SIZE];
} Board;

void initializeBoard(Board *board);
bool placeShip(Board *board, Ship ship);
void printBoard(Board *board);
int getRandom(int min, int max);
void generateBoard(Board *board);
void boardToString(Board *board, char *str);

#endif /* BOARD_H */