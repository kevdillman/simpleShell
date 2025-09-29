# Shell Build

SHELL:=bash

.DEFAULT_GOAL := run

.PHONY: all
all : simpleShell

simpleShell : simpleShell.c
	g++ simpleShell.c -o simpleShell.o

.PHONY: run
run : simpleShell
	ulimit -u 250; clear -x; ./simpleShell.o

.PHONY: clean
clean :
	rm -f simpleShell.o
