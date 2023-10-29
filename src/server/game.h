#ifndef GAME_H
#define GAME_H

#include "board.h"
#include "serverUtils.h"
#include <stdbool.h>

void initializeGame(Player *player1, Player *player2, char **msg1, char **msg2);
int makeShot(Board *opponentBoard, int x, int y);
bool hasPlayerWon(Board *opponentBoard);

#endif /* GAME_H */