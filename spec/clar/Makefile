########################################################################
# tools
########################################################################

CC = gcc

########################################################################
# flags
########################################################################

CFLAGS = -g -I. -I../../src -Wall

########################################################################
# files
########################################################################

CURRENT_MAKEFILE := $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST))
TEST_DIRECTORY := $(abspath $(dir $(CURRENT_MAKEFILE)))
CLAR_PATH := $(TEST_DIRECTORY)

objects = main.o hash.o

########################################################################
# targets
########################################################################

all: clar_test test

clar_test: $(objects) clar_test.h clar.suite
	$(CC) $(CFLAGS) -o $@ $(CLAR_PATH)/clar.c $(objects)

test:
	./clar_test

clar.suite:
	python "generate.py" .

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o clar.suite .clarcache clar_test clar_test.dSYM
