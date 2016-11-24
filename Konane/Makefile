SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall -std=c99
LDLIBS=-pthread

.PHONY: all clean

all: konane

konane: konane.o tree.o queue.o linklist.o tree.h

konane.o: konane.c tree.h
tree.o: tree.c tree.h queue.h

queue.o: queue.c queue.h linklist.h
linklist.o: linklist.c linklist.h

clean:
	$(RM) konane konane.o tree.o queue.o linklist.o