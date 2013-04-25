CC = gcc
FLAGS = -I../include -L../lib -lmt

all: cli

cli: cli.c
	$(CC) $(FLAGS) $< -o $@

clean:
	rm -f cli
