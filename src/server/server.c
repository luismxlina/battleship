#include "serverUtils.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
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

    // Crear el socket del servidor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("Error al crear el socket del servidor");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Vincular el socket a la dirección del servidor
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Error al vincular el socket a la dirección del servidor");
        exit(EXIT_FAILURE);
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 2) == 0)
        printf("Escuchando...\n");
    else
    {
        perror("Error al escuchar conexiones entrantes");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(serverSocket, &readfds);
    FD_SET(0, &readfds);

    // Inicializar las partidas y otros datos necesarios
    while (true)
    {
        // Aceptar conexiones de clientes
        if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrSize)) < 0)
        {
            perror("Error al aceptar la conexión del cliente");
            exit(EXIT_FAILURE);
        }

        // Inicializar un nuevo cliente
        Client newClient = initializeClient(clientSocket);
        bzero(buffer, sizeof(buffer));
        // strcpy(buffer, "Bienvenido a Battleship Online\n");
        const char *bannerFile = "battleship.txt";
        char *banner = readFile(bannerFile);
        if (banner == NULL)
        {
            perror("Error al leer el archivo del banner");
            free(banner);
            exit(EXIT_FAILURE);
        }
        strcpy(buffer, banner);
        if (send(clientSocket, buffer, sizeof(buffer), 0) == -1)
        {
            perror("Error en la operación de send");
            exit(1);
        }

        // Lógica de autenticación y emparejamiento de clientes

        // Crear y gestionar una nueva partida
        // ...

        // Cerrar el socket del cliente una vez que la partida haya terminado
        close(clientSocket);
    }

    // Cerrar el socket del servidor
    close(serverSocket);

    return 0;
}
