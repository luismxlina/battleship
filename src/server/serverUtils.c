#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "serverUtils.h"

// Función para inicializar un nuevo cliente
Client initializeClient(int socket)
{
    Client newClient;
    newClient.socket = socket;
    socklen_t addrSize = sizeof(struct sockaddr_in);
    getpeername(socket, (struct sockaddr *)&newClient.address, &addrSize);
    return newClient;
}

char *readfile(const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long fileSize = ftell(archivo);
    fseek(archivo, 0, SEEK_SET);

    char *contenido = (char *)malloc(fileSize + 1);

    if (contenido == NULL)
    {
        fclose(archivo);
        return NULL;
    }

    size_t bytesRead = fread(contenido, 1, fileSize, archivo);
    contenido[bytesRead] = '\0';

    fclose(archivo);
    return contenido;
}
