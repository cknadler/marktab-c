# Tools
CC = gcc

# Flags
INCLUDE = -I../../include/ -I../../src/ -I.
CFLAGS = $(INCLUDE) -g -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=gnu99
LDFLAGS = -L../../lib/

# Files
TEST_SRCS = $(wildcard *.c)
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_BINS = $(TEST_SRCS:.c=.test)

# Targets
all: $(TEST_BINS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.test: %.o
	$(CC) $(LDFLAGS) $< -lmt -lcheck -o $@

clean:
	rm -rf *.o *.test

test:
	./$(TEST_BINS)
