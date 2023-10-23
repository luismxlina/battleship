#include "board.h"
#include "auth.h"
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd;
	struct sockaddr_in sockname;
	char buffer[100];
	socklen_t len_sockname;
	int salir = 0;
	
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}

	

	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del 
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  inet_addr("127.0.0.1");

	/* ------------------------------------------------------------------
		Se solicita la conexión con el servidor
	-------------------------------------------------------------------*/
	len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		perror ("Error de conexión");
		exit(1);
	}

	printf("Conectado al servidor.\n");

	/* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	srand(time(NULL));
    Board playerBoard;

    auth();

    generateBoard(&playerBoard);

    printf("Tablero del Jugador:\n");
    printBoard(&playerBoard);

    char boardString[BOARD_SIZE * BOARD_SIZE + 1]; // +1 para el carácter nulo
    boardToString(&playerBoard, boardString);

    printf("\nTablero en forma de cadena:\n");
    printf("%s\n", boardString);
	
	// Cerrar el socket del cliente	
	close(sd);
	return 0;
}
