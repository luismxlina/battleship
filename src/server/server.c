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
    int output;
    Client clients[MAX_CLIENTS];
    int numClients = 0;

    int i, j, k;
    int received;
    char identifier[MSG_SIZE];

    int on, ret;

    // Crear el socket del servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    on = 1;
    ret = setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (ret < 0)
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
    if (!listen(serverSocket, 2) == 0)
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
        output = select(FD_SETSIZE, &auxfds, NULL, NULL, NULL);

        if (output > 0)
        {
            for (i = 0; i < FD_SETSIZE; i++)
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
                                clients[numClients].socket = clientSocket;
                                numClients++;
                                FD_SET(clientSocket, &readfds);

                                strcpy(buffer, "Bienvenido a Battleship\nEscriba la opción que desee:\n1. Iniciar sesión\n2. Registrarse\n3. Salir\n");

                                send(clientSocket, buffer, sizeof(buffer), 0);

                                for (j = 0; j < (numClients - 1); j++)
                                {

                                    bzero(buffer, sizeof(buffer));
                                    sprintf(buffer, "Nuevo Cliente conectado en <%d>", clientSocket);
                                    send(clients[j].socket, buffer, sizeof(buffer), 0);
                                }
                            }
                            else
                            {
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer, "Demasiados clientes conectados\n");
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

                            for (j = 0; j < numClients; j++)
                            {
                                bzero(buffer, sizeof(buffer));
                                strcpy(buffer, "Desconexión servidor\n");
                                send(clients[j].socket, buffer, sizeof(buffer), 0);
                                close(clients[j].socket);
                                FD_CLR(clients[j].socket, &readfds);
                            }
                            close(serverSocket);
                            exit(EXIT_FAILURE);
                        }
                        // Mensajes que se quieran mandar a los clientes (implementar)
                    }
                    else
                    {
                        bzero(buffer, sizeof(buffer));

                        received = recv(i, buffer, sizeof(buffer), 0);

                        if (received > 0)
                        {

                            if (strcmp(buffer, "3\n") == 0)
                            {
                                exitClient(i, &readfds, &numClients, clients);
                            }
                            else
                            {

                                sprintf(identifier, "<%d>: %s", i, buffer);
                                bzero(buffer, sizeof(buffer));

                                strcpy(buffer, identifier);

                                printf("%s\n", buffer);

                                for (j = 0; j < numClients; j++)
                                    if (clients[j].socket != i)
                                        send(clients[j].socket, buffer, sizeof(buffer), 0);
                            }
                        }
                        // Si el cliente introdujo ctrl+c
                        if (received == 0)
                        {
                            printf("El socket %d, ha introducido ctrl+c\n", i);
                            // Eliminar ese socket
                            exitClient(i, &readfds, &numClients, clients);
                        }
                    }
                }
            }
        }
    }

    close(serverSocket);
    return 0;
}

// Aceptar conexiones de clientes
//     if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize)) < 0)
//     {
//         perror("Error al aceptar la conexión del cliente");
//         exit(EXIT_FAILURE);
//     }

//     // Inicializar un nuevo cliente
//     Client newClient = initializeClient(clientSocket);
//     bzero(buffer, sizeof(buffer));
//     // strcpy(buffer, "Bienvenido a Battleship Online\n");
//     const char *bannerFile = "battleship.txt";
//     char *banner = readFile(bannerFile);
//     if (banner == NULL)
//     {
//         perror("Error al leer el archivo del banner");
//         free(banner);
//         exit(EXIT_FAILURE);
//     }
//     strcpy(buffer, banner);
//     if (send(clientSocket, buffer, sizeof(buffer), 0) == -1)
//     {
//         perror("Error en la operación de send");
//         exit(1);
//     }

//     // Lógica de autenticación y emparejamiento de clientes

//     // Crear y gestionar una nueva partida
//     // ...

//     // Cerrar el socket del cliente una vez que la partida haya terminado
//     close(clientSocket);
// }

// // Cerrar el socket del servidor
// close(serverSocket);