CC = gcc
CFLAGS = 
LIBS = -lssl -lcrypto

# Add -Wno-deprecated-declarations flag for auth.c
AUTH_CFLAGS = -Wno-deprecated-declarations

SRCS = server.c serverUtils.c board.c auth.c game.c list.c
OBJS = $(SRCS:.c=.o)
TARGET = server

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

# Use separate rules for auth.c to apply custom CFLAGS
auth.o: auth.c
	$(CC) -c $< $(AUTH_CFLAGS)

# Default rule for other .c files
.c.o:
	$(CC) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
