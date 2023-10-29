#include "auth.h"
#include "game.h"
#include "list.h"
#include "serverUtils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    fd_set readfds, auxfds;

    char buffer[MSG_SIZE];
    char identifier[MSG_SIZE];
    int on, ret_select;

    int numClients = 0;
    int numGames = 0;
    int received;

    List *players = NULL;

    // Crear el socket del servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("Error al configurar el socket del servidor");
        exit(EXIT_FAILURE);
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Vincular el socket a la dirección del servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Error al vincular el socket a la dirección del servidor");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 2) < 0)
    {
        perror("Error al escuchar conexiones entrantes");
        exit(EXIT_FAILURE);
    }
    printf("Servidor esperando conexiones...\n");

    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(serverSocket, &readfds);
    FD_SET(0, &readfds);

    signal(SIGINT, signalHandler);

    // Inicializar las partidas y otros datos necesarios
    while (true)
    {
        auxfds = readfds;
        ret_select = select(FD_SETSIZE, &auxfds, NULL, NULL, NULL);

        if (ret_select > 0)
        {
            for (int i = 0; i < FD_SETSIZE; i++)
            {
                // Buscamos el socket por el que se ha establecido la comunicación
                if (FD_ISSET(i, &auxfds))
                {

                    if (i == serverSocket)
                    {

                        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize)) == -1)
                        {
                            perror("Error aceptando peticiones");
                        }
                        else
                        {
                            if (numClients < MAX_CLIENTS)
                            {
                                Player *newPlayer = initializePlayer(clientSocket);
                                pushBack(&players, newPlayer);
                                numClients++;
                                printf("Nuevo jugador conectado: %d/%d\n", clientSocket, numClients);
                                FD_SET(clientSocket, &readfds);

                                strcpy(buffer, "+Ok. Usuario conectado\n");
                                send(clientSocket, buffer, sizeof(buffer), 0);
                            }
                            else
                            {
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer, "-Err. Demasiados usuarios conectados\n");
                                send(clientSocket, buffer, sizeof(buffer), 0);
                                close(clientSocket);
                            }
                        }
                    }
                    else if (i == 0)
                    {
                        // Se ha introducido información de teclado
                        bzero(buffer, sizeof(buffer));
                        fgets(buffer, sizeof(buffer), stdin);

                        // Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                        if (strcmp(buffer, "SALIR\n") == 0)
                        {

                            for (int j = 0; j < numClients; j++)
                            {
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer, "Desconexión servidor\n");
                                send(players->item->socket, buffer, sizeof(buffer), 0);
                                close(players->item->socket);
                                FD_CLR(players->item->socket, &readfds);
                                removeItem(&players, players->item->socket);
                            }
                            close(serverSocket);
                            exit(EXIT_FAILURE);
                        }
                        else
                        {
                            for (int j = 0; j < numClients; j++)
                            {
                                send(players->item->socket, buffer, sizeof(buffer), 0);
                            }
                        }
                    }
                    // Mensaje de un cliente antiguo
                    else
                    {
                        bzero(buffer, sizeof(buffer));

                        received = recv(i, buffer, sizeof(buffer), 0);

                        if (received > 0)
                        {
                            Player *currentPlayer = getPlayer(players, i);
                            int playerStatus = currentPlayer->status;
                            if (buffer[strlen(buffer) - 1] == '\n')
                            {
                                buffer[strlen(buffer) - 1] = '\0';
                            }
                            printf("Jugador[sd: %d, estado: %d] mensaje: <%s>\n", i, playerStatus, buffer);
                            if (strcmp(buffer, "SALIR\n") == 0)
                            {
                                printf("Jugador <%d> saliendo...\n", i);
                                exitClient(currentPlayer, &readfds, &numClients, &players);
                            }
                            else
                            {
                                char *instruction = strtok(buffer, " ");
                                if (strlen(buffer) == 0)
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "-Err. Instrucción no válida\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                                else if (playerStatus == 0)
                                {
                                    if (strcmp(instruction, "USUARIO") == 0)
                                    {
                                        if ((instruction = strtok(NULL, " ")) != NULL)
                                        {
                                            if (findUser(FILENAME, instruction) == 1 &&
                                                findPlayerByName(players, instruction) == 0)
                                            {
                                                currentPlayer->status = 1;
                                                strcpy(currentPlayer->name, instruction);
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+Ok. Usuario correcto\n");
                                            }
                                            else if (findPlayerByName(players, instruction) == 1)
                                            {
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "-Err. Usuario ya conectado\n");
                                            }
                                            else
                                            {
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "-Err. Usuario incorrecto\n");
                                            }
                                        }
                                        else
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "-Err. Instrucción no válida\n");
                                        }
                                    }
                                    else if (strcmp(instruction, "REGISTRO") == 0)
                                    {
                                        int correct = 0, error = 0, count = 0;
                                        char user[250];
                                        char psswd[MSG_SIZE];
                                        while (count < 4 && error == 0)
                                        {
                                            char *line;
                                            if ((instruction = strtok(NULL, " ")) != NULL && strlen(instruction) > 0)
                                            {
                                                if (count == 0 && strcmp(instruction, "-u") == 0)
                                                {
                                                }
                                                else if (count == 1)
                                                {
                                                    strcpy(user, instruction);
                                                }
                                                else if (count == 2 && strcmp(instruction, "-p") == 0)
                                                {
                                                }
                                                else if (count == 3)
                                                {
                                                    strcpy(psswd, instruction);
                                                }
                                                else
                                                {
                                                    error = 1;
                                                }
                                                count++;
                                            }
                                            else
                                            {
                                                error = 1;
                                            }
                                        }
                                        if (findUser(FILENAME, user) == 1 && error == 0)
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "-Err. Usuario ya registrado\n");
                                        }
                                        else if (error == 0)
                                        {
                                            createUser(FILENAME, user, psswd);
                                            currentPlayer->status = 2;
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "+Ok. Usuario registrado\n");
                                        }
                                        else
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "-Err. Error en los argumentos\n");
                                        }
                                    }
                                    else
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "-Err. Instrucción no válida\n");
                                    }
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                                else if (playerStatus == 1)
                                {
                                    if (strcmp(instruction, "PASSWORD") == 0)
                                    {
                                        if ((instruction = strtok(NULL, "\0")) != NULL)
                                        {
                                            if (checkCredentials(FILENAME, currentPlayer->name, instruction) == 1)
                                            {
                                                currentPlayer->status = 2;
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+OK. Usuario validado\n");
                                            }
                                            else
                                            {
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "-Err. Error en la validación\n");
                                            }
                                        }
                                        else
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "-Err. Instrucción no válida\n");
                                        }
                                    }
                                    else
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "-Err. Instrucción no válida\n");
                                    }
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                                else if (playerStatus == 2)
                                {
                                    if (strcmp(instruction, "INICIAR-PARTIDA") == 0)
                                    {
                                        Player *rival = assignPlayer(players);
                                        if (numGames < MAX_GAMES)
                                        {
                                            if (rival != NULL)
                                            {
                                                numGames++;
                                                bzero(buffer, sizeof(buffer));
                                                char msg1[MSG_SIZE], msg2[MSG_SIZE];
                                                strcpy(buffer, "+Ok. Empieza la partida");
                                                initializeGame(currentPlayer, rival, msg1, msg2);
                                                send(currentPlayer->socket, msg1, sizeof(msg1), 0);
                                                send(rival->socket, msg2, sizeof(msg2), 0);

                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+Ok. Turno de partida\n");
                                                send(rival->socket, buffer, sizeof(buffer), 0);
                                            }
                                            else
                                            {
                                                currentPlayer->status = 3;
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "+Ok. Esperando jugadores\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                        }
                                        else
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "-Err. Número máximo de partidas\n");
                                            send(i, buffer, sizeof(buffer), 0);
                                        }
                                    }
                                    else
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "-Err. Instrucción no válida\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                    }
                                }
                                else if (playerStatus == 3)
                                {
                                    bzero(buffer, sizeof(buffer));
                                    strcpy(buffer, "+Ok. Instrucción no válida\n");
                                    send(i, buffer, sizeof(buffer), 0);
                                }
                                else if (playerStatus == 4)
                                {
                                    Game *game = currentPlayer->game;
                                    Player *rival = (game->turn % 2 == 0) ? game->player1 : game->player2;

                                    if (strcmp(instruction, "DISPARO") == 0)
                                    {
                                        if (currentPlayer->socket == rival->socket)
                                        {
                                            bzero(buffer, sizeof(buffer));
                                            strcpy(buffer, "-Err. Debe esperar su turno\n");
                                            send(i, buffer, sizeof(buffer), 0);
                                        }
                                        else
                                        {
                                            if ((instruction = strtok(NULL, "\0")) != NULL)
                                            {
                                                char *row = strtok(NULL, ",");
                                                char *column = strtok(NULL, "\0");
                                                int x = (int)(row[0] - 'A');
                                                int y = atoi(column) - 1;
                                                int res = makeShot(&game->board1, x, y);
                                                // Devuelve -1 si las coordenadas están fuera del tablero
                                                if (res == -1)
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Coordenadas fuera del tablero\n");
                                                    send(i, buffer, sizeof(buffer), 0);
                                                }
                                                // Devuelve 0 si ya se hizo un disparo en esa casilla
                                                else if (res == 0)
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "-Err. Ya se hizo un disparo en esa casilla\n");
                                                    send(i, buffer, sizeof(buffer), 0);
                                                }
                                                // Devuelve 1 si se acertó un barco
                                                else if (res == 1)
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Tocado\n");
                                                    send(i, buffer, sizeof(buffer), 0);
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Tocado rival\n");
                                                    send(rival->socket, buffer, sizeof(buffer), 0);
                                                    if (hasPlayerWon(&game->board1))
                                                    {
                                                        bzero(buffer, sizeof(buffer));
                                                        strcpy(buffer, "+Ok. Has ganado la partida\n");
                                                        send(i, buffer, sizeof(buffer), 0);
                                                        bzero(buffer, sizeof(buffer));
                                                        strcpy(buffer, "+Ok. Has perdido la partida\n");
                                                        send(rival->socket, buffer, sizeof(buffer), 0);
                                                        currentPlayer->status = 2;
                                                        rival->status = 2;
                                                        numGames--;
                                                    }
                                                }
                                                // Devuelve 2 si se disparó al agua
                                                else if (res == 2)
                                                {
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Agua\n");
                                                    send(i, buffer, sizeof(buffer), 0);
                                                    bzero(buffer, sizeof(buffer));
                                                    strcpy(buffer, "+Ok. Agua rival\n");
                                                    send(rival->socket, buffer, sizeof(buffer), 0);
                                                }
                                            }
                                            else
                                            {
                                                bzero(buffer, sizeof(buffer));
                                                strcpy(buffer, "-Err. Indique las coordenadas\n");
                                                send(i, buffer, sizeof(buffer), 0);
                                            }
                                        }
                                    }
                                    else
                                    {
                                        bzero(buffer, sizeof(buffer));
                                        strcpy(buffer, "-Err. Instrucción no válida\n");
                                        send(i, buffer, sizeof(buffer), 0);
                                    }
                                }
                                // Si el cliente introdujo ctrl+c
                                if (received == 0)
                                {
                                    printf("El socket %d, ha introducido ^C\n", i);
                                    Player *player = getPlayer(players, i);
                                    if (player->status == 4)
                                    {
                                        numGames--;
                                    }
                                    // Eliminar ese socket
                                    exitClient(player, &readfds, &numClients, &players);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    close(serverSocket);
    return 0;
}