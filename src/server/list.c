#include "list.h"
#include "serverUtils.h"

List *newItem()
{
    return (List *)malloc(sizeof(List));
}

void pushBack(List **head, Player *player)
{
    List *new = newItem();
    new->item = player;
    List *aux = *head;
    if (aux == NULL)
    {
        *head = new;
        return;
    }
    while (aux->next != NULL)
    {
        aux = aux->next;
    }
    aux->next = new;
}

void pushFront(List **head, Player *player)
{
    List *new = newItem();
    new->item = player;
    new->next = *head;
    *head = new;
}

void removeItem(List **head, int sd)
{
    List *aux = *head;
    if ((*head)->item->socket == sd)
    {
        *head = (*head)->next;
        free(aux);
        return;
    }
    while (aux->next->item->socket != sd)
    {
        aux = aux->next;
    }
    aux->next = aux->next->next;
}

int listSize(List *head)
{
    int size = 0;
    List *aux = head;
    while (aux != NULL)
    {
        size++;
        aux = aux->next;
    }
    return size;
}

Player *getPlayer(List *head, int sd)
{
    List *aux = head;
    while (aux != NULL)
    {
        if (aux->item->socket == sd)
        {
            return aux->item;
        }
        aux = aux->next;
    }
    return NULL;
}

int findPlayerByName(List *head, char *name)
{
    List *aux = head;
    while (aux != NULL)
    {
        if (strcmp(aux->item->name, name) == 0)
        {
            return 1;
        }
        aux = aux->next;
    }
    return 0;
}
Player *assingPlayer(List *head)
{
    List *aux = head;
    while (aux != NULL)
    {
        if (aux->item->status == 3)
        {
            return aux->item;
        }
        aux = aux->next;
    }
    return NULL;
}

void printList(List *head)
{
    List *aux = head;
    while (aux != NULL)
    {
        printf("Jugador[sd:%d, estado:%d, nombre:%s, partida:%p]\n",
               aux->item->socket, aux->item->status, aux->item->name, aux->item->game);
        aux = aux->next;
    }
}