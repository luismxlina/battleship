# Cliente-Servidor de Batalla Naval

Este es un ejemplo de un programa de cliente-servidor para jugar al juego de batalla naval. El servidor permite a varios clientes conectarse y jugar partidas de batalla naval entre ellos. A continuación, se describe la arquitectura general del programa y cómo compilarlo.

## Arquitectura del Programa

El programa cliente-servidor de batalla naval consta de dos partes principales:

1. **Cliente**: El cliente es una aplicación que permite a un usuario interactuar con el servidor. Puede realizar acciones como registrarse, unirse a una partida, realizar disparos y recibir actualizaciones sobre el estado del juego. El cliente se comunica con el servidor a través de sockets TCP.

2. **Servidor**: El servidor es el componente central que administra las conexiones de los clientes, coordina las partidas y mantiene el estado del juego. El servidor escucha las conexiones entrantes en un puerto específico y se encarga de manejar múltiples conexiones de clientes simultáneamente. También almacena información sobre los usuarios y las partidas en archivos encriptados.

La comunicación entre el cliente y el servidor se basa en mensajes de texto simples. Los comandos y respuestas se envían como cadenas de texto a través del socket.

## Compilación del Programa

El programa se divide en dos partes, el cliente y el servidor. Para compilarlos, puedes usar archivos Makefile proporcionados en las carpetas `src/client` y `src/server`. Asegúrate de tener instaladas las siguientes bibliotecas antes de compilar:

- `openssl`: Para la persistencia de usuarios y el cifrado de datos en el archivo.

Si no tienes las bibliotecas instaladas, puedes instalarlas en sistemas basados en Debian/Ubuntu ejecutando:

```bash
sudo apt-get install openssl libssl-dev
```

A continuación, los pasos generales para compilar el cliente y el servidor:

### Compilación del Cliente

1. Abre una terminal y navega hasta la carpeta `src/client`.

2. Ejecuta el siguiente comando para compilar el cliente:

   ```bash
   make
   ```

3. El archivo ejecutable del cliente se generará en la misma carpeta.

### Compilación del Servidor

1. Abre una terminal y navega hasta la carpeta `src/server`.

2. Ejecuta el siguiente comando para compilar el servidor:

   ```bash
   make
   ```

3. El archivo ejecutable del servidor se generará en la misma carpeta.

## Ejecución del Programa

Para ejecutar el cliente y el servidor, sigue los siguientes pasos:

### Ejecución del Servidor

1. Asegúrate de estar en la carpeta `src/server`.

2. Ejecuta el servidor con el siguiente comando:

   ```bash
   ./server
   ```

El servidor se iniciará y esperará conexiones de clientes en el puerto especificado en el código.

### Ejecución del Cliente

1. Asegúrate de estar en la carpeta `src/client`.

2. Ejecuta el cliente con el siguiente comando:

   ```bash
   ./client
   ```

El cliente se conectará al servidor y te permitirá interactuar con él a través de comandos de texto.

## Contribuyendo

Si deseas contribuir a este proyecto, puedes hacerlo creando bifurcaciones (forks) y enviando solicitudes de extracción (pull requests) con mejoras o correcciones.

## Licencia

Este programa se distribuye bajo la Licencia MIT. Consulta el archivo `LICENSE` para obtener más información.

¡Diviértete jugando batalla naval con tus amigos en este servidor cliente-servidor!