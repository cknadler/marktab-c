CC = gcc
YACC = bison
LEX = flex

CFLAGS = -g -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=gnu99
LDFLAGS =
YFLAGS = -vyd
LFLAGS =

BIN_DIR = bin

MARKTAB_DIR = src

MARKTAB_SRCS = \
mt_string.c \
mt.c \
mt_object.c \
mt_note.c \
mt_chord.c \
mt_transition.c \
mt_sequence.c \
mt_symbol.c \
mt_queue.c \
mtr.c \
mt_conf.c \
mt_stack.c \
mt_output.c \
mt_hash.c \
mt_pair.c \
mt_tree.c \
mt_error.c

MARKTAB_OBJS = $(addprefix $(MARKTAB_DIR)/,$(MARKTAB_SRCS:.c=.o))

LLYY_OBJS = $(addprefix $(MARKTAB_DIR)/,mt_parser.o mt_lexer.o)

all: marktab

marktab: bin $(LLYY_OBJS) $(MARKTAB_OBJS)
	$(CC) $(LDFLAGS) $(LLYY_OBJS) $(MARKTAB_OBJS) -o $(BIN_DIR)/marktab

bin:
	mkdir -p $(BIN_DIR)

$(MARKTAB_DIR)/mt_parser.o: $(MARKTAB_DIR)/marktab.y
	$(YACC) $(YFLAGS) $(MARKTAB_DIR)/marktab.y -o $(MARKTAB_DIR)/mt_parser.c
	$(CC) $(CFLAGS) -c $(MARKTAB_DIR)/mt_parser.c -o $@

$(MARKTAB_DIR)/mt_lexer.o: $(MARKTAB_DIR)/marktab.l
	$(LEX) $(LFLAGS) -o $(MARKTAB_DIR)/mt_lexer.c $(MARKTAB_DIR)/marktab.l
	$(CC) $(CFLAGS) -c $(MARKTAB_DIR)/mt_lexer.c -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test:
	rcomp test

clean:
	rm -rf $(BIN_DIR)
	rm -f $(MARKTAB_DIR)/*.o
	rm -f $(MARKTAB_DIR)/mt_lexer.*
	rm -f $(MARKTAB_DIR)/mt_parser.*
