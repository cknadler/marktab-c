AM_CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=c99 -D_POSIX_SOURCE -O0 -g
AM_LDFLAGS =
AM_YFLAGS = -d -v
AM_LFLAGS =

bin_PROGRAMS = marktab testbed

marktab_SOURCES = y.tab.y lex.yy.l mt_string.c mt.c mt_object.c mt_note.c \
mt_chord.c mt_transition.c mt_sequence.c mt_queue.c mtr.c mt_config.c

testbed_SOURCES = mt_stack.c testbed.c
