AM_CFLAGS = -Wall -Wextra -Werror -Wno-unused-parameter -Wno-unused-function -std=gnu99 -O3 -g
AM_YFLAGS = -d -v
AM_LFLAGS =

bin_PROGRAMS = marktab

marktab_LDADD = -lm

marktab_SOURCES = \
y.tab.y \
lex.yy.l \
mt_string.c \
mt.c \
mt_object.c \
mt_note.c \
mt_chord.c \
mt_transition.c \
mt_sequence.c \
mt_queue.c \
mtr.c \
mt_config.c \
mt_stack.c \
mt_output.c \
mt_hash.c \
mt_pair.c \
mt_tree.c
