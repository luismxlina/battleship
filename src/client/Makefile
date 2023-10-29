CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99
LDFLAGS=-lm

all: cliente clientecoño cliente_paco

cliente: client.o board.o
    $(CC) $(CFLAGS) -o cliente client.o board.o $(LDFLAGS)

clientecoño: clientecoño.o board.o
    $(CC) $(CFLAGS) -o clientecoño clientecoño.o board.o $(LDFLAGS)

cliente_paco: cliente_paco.o board.o
    $(CC) $(CFLAGS) -o cliente_paco cliente_paco.o board.o $(LDFLAGS)

client.o: client.c board.h
    $(CC) $(CFLAGS) -c client.c

clientecoño.o: clientecoño.c board.h
    $(CC) $(CFLAGS) -c clientecoño.c

cliente_paco.o: cliente_paco.c board.h
    $(CC) $(CFLAGS) -c cliente_paco.c

board.o: board.c board.h
    $(CC) $(CFLAGS) -c board.c

clean:
    rm -f *.o cliente clientecoño cliente_paco