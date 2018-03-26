CC = gcc
CFLAGS = -Wall -std=c11 -g -pthread

BIN = ./bin/
SRC = ./src/
INC = ./include/

all: $(BIN)LinkedListAPI.o $(SRC)holes.c $(SRC)dine.c
	$(CC) $(CFLAGS) -Iinclude $(SRC)holes.c $(BIN)LinkedListAPI.o -o holes
	$(CC) $(CFLAGS) -Iinclude $(SRC)dine.c $(BIN)LinkedListAPI.o -o dine

$(BIN)LinkedListAPI.o: $(SRC)LinkedListAPI.c
	$(CC) $(CFLAGS) -c $(SRC)LinkedListAPI.c -Iinclude -o $(BIN)LinkedListAPI.o
	
clean: 
	rm -f $(BIN)*
	rm -f holes
	rm -f dine
