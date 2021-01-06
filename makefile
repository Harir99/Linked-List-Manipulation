CC=gcc 
CFLAGS=-std=c99 -Wall -pedantic -DLIST

all: list.o 

list: list.c list.h 
	$(CC) $(CFLAGS) list.c -o list

list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c -o list.o

clean:
	rm -f *.o 
