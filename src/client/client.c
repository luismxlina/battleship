#include "board.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int main()
{

    /*----------------------------------------------------
        Descriptor del socket y buffer de datos
    -----------------------------------------------------*/
    int sd;
    struct sockaddr_in sockname;
    char buffer[1000];
    char aux[1000];
    Board *board = (Board *)malloc(sizeof(Board));
    socklen_t len_sockname;
    fd_set readfds, auxfds;
    int salida;
    int end = 0;

    /* --------------------------------------------------
        Se abre el socket
    ---------------------------------------------------*/
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd == -1)
    {
        perror("No se puede abrir el socket cliente\n");
        exit(1);
    }

    /* ------------------------------------------------------------------
        Se rellenan los campos de la estructura con la IP del
        servidor y el puerto del servicio que solicitamos
    -------------------------------------------------------------------*/
    sockname.sin_family = AF_INET;
    sockname.sin_port = htons(2060);
    sockname.sin_addr.s_addr = inet_addr("127.0.0.1");

    /* ------------------------------------------------------------------
        Se solicita la conexión con el servidor
    -------------------------------------------------------------------*/
    len_sockname = sizeof(sockname);

    if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
    {
        perror("Error de conexión");
        exit(1);
    }

    // Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);

    FD_SET(0, &readfds);
    FD_SET(sd, &readfds);

    /* ------------------------------------------------------------------
        Se transmite la información
    -------------------------------------------------------------------*/
    do
    {
        auxfds = readfds;
        salida = select(sd + 1, &auxfds, NULL, NULL, NULL);

        if (FD_ISSET(sd, &auxfds))
        {
            bzero(buffer, sizeof(buffer));
            recv(sd, buffer, sizeof(buffer), 0);
            if (strstr(buffer, "+Ok. Empieza la partida") != NULL)
            {
                printf("+Ok. Empieza la partida.\n");
                strcpy(aux, buffer + 23);
                stringToBoard(board, aux);
                printBoard(board);
            }
            else if (strstr(buffer, "+Ok. Nuevo tablero") != NULL)
            {

                printf("+Ok. Nuevo tablero.\n");
                strcpy(aux, buffer + 18);
                stringToBoard(board, aux);
                printBoard(board);
            }
            else
            {
                printf("%s\n", buffer);
            }
            if (strcmp(buffer, "-Err. Demasiados usuarios conectados\n") == 0)
            {
                printf("-Err. Demasiados usuarios conectados. Inténtelo más tarde.\n");
                end = 1;
            }
            if (strcmp(buffer, "+Ok. Desconexión procesada.\n") == 0)
            {
                printf("+Ok. Desconexión procesada.\n");
                end = 1;
            }
        }
        else
        {
            if (FD_ISSET(0, &auxfds))
            {
                bzero(buffer, sizeof(buffer));
                fgets(buffer, sizeof(buffer), stdin);
                send(sd, buffer, sizeof(buffer), 0);
                if (strcmp(buffer, "SALIR\n") == 0)
                    end = 1;
            }
        }

    } while (end == 0);

    // Cerrar el socket del cliente
    close(sd);
    return 0;
}
