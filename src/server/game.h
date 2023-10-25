#ifndef GAME_H
#define GAME_H

#include "board.h"
#include <stdbool.h>

bool makeShot(Board *opponentBoard, int x, int y);
bool hasPlayerWon(Board *opponentBoard);

#endif /* GAME_H */