CC = gcc
CFLAGS = -Wall -pthread

SRC = $(wildcard *.c)
BIN = $(SRC:.c=)

all: $(BIN)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BIN)
