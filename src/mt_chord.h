#ifndef MT_CHORD_H
#define MT_CHORD_H

#include "mt_type.h"

struct MtNote;

typedef struct MtChord
{
  MtModifierType modifier;
  struct MtNote* notes[MAX_FRETS];
} MtChord;

MtChord* mt_chord_new(struct MtNote* notes[], MtModifierType modifier);

void mt_chord_free(MtChord* chord);

#endif
