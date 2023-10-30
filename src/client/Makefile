CC = gcc
CFLAGS =
LIBS =

SRCS = client.c board.c
OBJS = $(SRCS:.c=.o)
TARGET = client

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.c.o:
	$(CC) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean