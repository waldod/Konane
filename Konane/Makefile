SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall -std=c99 -g
LDLIBS=-pthread -lm

.PHONY: all clean

all: konane

konane: konane.o tree.o queue.o alphabeta.o linklist.o tree.h

konane.o: konane.c tree.h alphabeta.h
alphabeta.o: alphabeta.c alphabeta.h tree.h
tree.o: tree.c tree.h queue.h
queue.o: queue.c queue.h linklist.h
linklist.o: linklist.c linklist.h

clean:
	$(RM) konane konane.o tree.o queue.o linklist.o
