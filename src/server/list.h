#ifndef LIST_H
#define LIST_H
#include "serverUtils.h"

List *newItem();
void pushBack(List **head, Player *player);
void pushFront(List **head, Player *player);
void removeItem(List **head, int sd);
int listSize(List *head);
Player *getPlayer(List *head, int sd);
int findPlayerByName(List *head, char *name);
Player *assignPlayer(List *head);
void printList(List *head);

#endif
