
CC=gcc
CFLAGS=-ansi -pedantic-errors -Wall -Wextra -g

all: master dumb-cat

master: master.c
	$(CC) $(CFLAGS) -o $@ $^

dumb-cat: dumb-cat.c
	$(CC) $(CFLAGS) -o $@ $^