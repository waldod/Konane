SHELL=/bin/bash
CC=gcc
CFLAGS=-Wall
LDLIBS=-pthread

.PHONY: konane clean

all: konane

konane: konane.o tree.o linklist.o queue.o
konane.o: konane.c tree.h
tree.o: tree.c tree.h queue.h
queue.o: queue.c queue.h linklist.h
linklist.o: linklist.c linklist.h

clean:
	$(RM) konane konane.o tree.o queue.o linklist.o
