########################################################################
# tools
########################################################################

CC = gcc
YACC = bison
LEX = flex
AR = ar

########################################################################
# flags
########################################################################

CFLAGS = -g -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=gnu99 -O3
LDFLAGS = -lm
ARFLAGS = -rcs
YFLAGS = -vyd
LFLAGS =

########################################################################
# files
########################################################################

BIN_DIR = bin
SRC_DIR = src
INCLUDE_DIR = include
EXAMPLES_DIR = examples

LIBMT_SRCS = \
mt_string.c \
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

EXAMPLE_SRCS = \
cli.c 

LIBMT_OBJS = $(addprefix $(SRC_DIR)/,mt_parser.o mt_lexer.o $(LIBMT_SRCS:.c=.o))

EXAMPLE_OBJS = $(addprefix $(EXAMPLES_DIR)/,$(EXAMPLE_SRCS:.c=.o))

########################################################################
# targets
########################################################################

all: libmt

examples: libmt $(EXAMPLE_OBJS)
	$(CC) $(LDFLAGS) $(EXAMPLE_OBJS) -o $(EXAMPLES_DIR)/marktab

libmt: $(LIBMT_OBJS)
	$(AR) $(ARFLAGS) $(INCLUDE_DIR)/libmt.a $<

$(SRC_DIR)/mt_parser.o: $(SRC_DIR)/marktab.y
	$(YACC) $(YFLAGS) $< -o $(SRC_DIR)/mt_parser.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/mt_parser.c -o $@
	@rm $(SRC_DIR)/mt_parser.c

$(SRC_DIR)/mt_lexer.o: $(SRC_DIR)/marktab.l
	$(LEX) $(LFLAGS) -o $(SRC_DIR)/mt_lexer.c $<
	$(CC) $(CFLAGS) -c $(SRC_DIR)/mt_lexer.c -o $@
	@rm $(SRC_DIR)/mt_lexer.c

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test:
	rcomp test

# Note: build_clean nukes the bin directory.
# Don't get attached to anything in there
clean_build:
	rm -f $(addprefix $(SRC_DIR)/,*.o mt_lexer.* mt_parser.*)

clean_test:
	rm -rf spec/rcomp/results

clean: clean_build clean_test
