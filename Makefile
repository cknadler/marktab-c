CC = gcc
YACC = bison
LEX = flex

CFLAGS = -g -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=gnu99
LDFLAGS = 
YFLAGS = -vyd
LFLAGS =

BIN_DIR = bin

MARKTAB_DIR = src
MARKTAB_SRCS = $(wildcard src/*.c)
MARKTAB_OBJS = $(MARKTAB_SRCS:.c=.o)

all: marktab

marktab: bin llyy $(MARKTAB_OBJS)
	$(CC) $(LDFLAGS) $(MARKTAB_OBJS) -o $(BIN_DIR)/marktab

bin:
	mkdir -p $(BIN_DIR)

llyy: $(MARKTAB_DIR)/mt_parser.o $(MARKTAB_DIR)/mt_lexer.o

$(MARKTAB_DIR)/mt_parser.o: $(MARKTAB_DIR)/marktab.y
	$(YACC) $(YFLAGS) $(MARKTAB_DIR)/marktab.y -o $(MARKTAB_DIR)/mt_parser.c
	$(CC) $(CFLAGS) -c $(MARKTAB_DIR)/mt_parser.c -o $@

$(MARKTAB_DIR)/mt_lexer.o: $(MARKTAB_DIR)/marktab.l
	$(LEX) $(LFLAGS) -o $(MARKTAB_DIR)/mt_lexer.c $(MARKTAB_DIR)/marktab.l
	$(CC) $(CFLAGS) -c $(MARKTAB_DIR)/mt_lexer.c -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)
	rm -f $(MARKTAB_DIR)/*.o
	rm -f $(MARKTAB_DIR)/mt_lexer.c
	rm -f $(MARKTAB_DIR)/mt_parser.h
	rm -f $(MARKTAB_DIR)/mt_parser.c
	rm -f $(MARKTAB_DIR)/mt_parser.output
	
