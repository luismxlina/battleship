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
#include <time.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd;
	struct sockaddr_in sockname;
	char buffer[250];
	char board[250];
	socklen_t len_sockname;
	fd_set readfds, auxfds;
	int salir;
	int end = 0;
	
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

	//Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    
    FD_SET(0,&readfds);
    FD_SET(sd,&readfds);

	/* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	do
	{
		 auxfds = readfds;
        salir = select(sd+1,&auxfds,NULL,NULL,NULL);
        //Tengo mensaje desde el servidor
        if(FD_ISSET(sd, &auxfds)){
            
            bzero(buffer,sizeof(buffer));
            recv(sd,buffer,sizeof(buffer),0);
			//printf("%s",buffer);
			if(strstr(buffer,"+Ok. Empieza la partida.") != NULL){
				strcpy(board,buffer+24);
				for(int i=0;i<sizeof(board);i++){
					if(board[i]==','){
						board[i]='|';
					}
					else if(board[i]==';'){
						board[i]='\n';
					}
					else if(board[i]=='-'){
						board[i]=' ';
					}
				}
				printf("+Ok. Empieza la partida.\n1|2|3|4|5|6|7\n%s",board);
			}
			else if(strstr(buffer,"+Ok. Nuevo tablero.") != NULL){
				strcpy(board,buffer+19);
				for(int i=0;i<sizeof(board);i++){
					if(board[i]==','){
						board[i]='|';
					}
					else if(board[i]==';'){
						board[i]='\n';
					}
					else if(board[i]=='-'){
						board[i]=' ';
					}
				}
				printf("+Ok. Nuevo board.\n1|2|3|4|5|6|7\n%s",board);
			}else{
            	printf("\n%s\n",buffer);
			}
            
            if(strcmp(buffer,"-Err. Demasiados usuarios conectados\n") == 0)
                end = 1;
            
            if(strcmp(buffer,"Desconexión servidor\n") == 0)
                end = 1;
        }
        else
        {
            //He introducido información por teclado
            if(FD_ISSET(0,&auxfds)){
                bzero(buffer,sizeof(buffer));
                
                fgets(buffer,sizeof(buffer),stdin);
                
                if(strcmp(buffer,"SALIR\n") == 0)
				{
                        end = 1;
                }
                send(sd,buffer,sizeof(buffer),0);   
            }   
        }
	} while (end == 0);
	
	// Cerrar el socket del cliente	
	close(sd);
	return 0;
}
