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

# cflags
INCLUDE = -Iinclude/
DEBUG = -g
ERROR = -Wall -Wextra -Werror -Werror -Wno-unused-parameter -Wno-unused-function
PEDANTIC = -pedantic -pedantic-errors
STANDARD = -std=gnu99
OPTIMIZATION = -O3
CFLAGS = $(INCLUDE) $(DEBUG) $(ERROR) $(PEDANTIC) $(STANDARD) $(OPTIMIZATION)

LDFLAGS = -lm
ARFLAGS = -rcs
YFLAGS = -vyd
LFLAGS =

########################################################################
# files
########################################################################

# Source Directories
SRC_DIR = src
INCLUDE_DIR = include
EXAMPLES_DIR = examples

# Test Directories
RCOMP_DIR = spec/rcomp
CLAR_DIR = spec/clar

LIBMT_SRCS = $(wildcard $(SRC_DIR)/*.c)

EXAMPLE_SRCS = $(EXAMPLES_DIR)/cli.c

LIBMT_OBJS = $(addprefix $(SRC_DIR)/,mt_parser.o mt_lexer.o) $(LIBMT_SRCS:.c=.o)

EXAMPLE_OBJS = $(EXAMPLE_SRCS:.c=.o)

########################################################################
# targets
########################################################################

all: libmt

examples: libmt $(EXAMPLE_OBJS)
	$(CC) $(LDFLAGS) $(EXAMPLE_OBJS) -o $(EXAMPLES_DIR)/marktab

libmt: $(INCLUDE_DIR)/libmt.a

# Library Target
$(INCLUDE_DIR)/libmt.a: $(LIBMT_OBJS)
	$(AR) $(ARFLAGS) $@ $<

# Parser Target
$(SRC_DIR)/mt_parser.o: $(SRC_DIR)/marktab.y
	$(YACC) $(YFLAGS) $< -o $(SRC_DIR)/mt_parser.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/mt_parser.c -o $@
	@rm $(SRC_DIR)/mt_parser.c

# Lexer Target
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
	rm -f $(INCLUDE_DIR)/libmt.a

clean_test:
	rm -rf $(RCOMP_DIR)/results
	rm -f $(addprefix $(CLAR_DIR)/,.clarcache clar.suite clar_test.h)

clean: clean_build clean_test

rebuild: clean all
