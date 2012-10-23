#ifndef MT_CHORD_H
#define MT_CHORD_H

#include "mt_type.h"
#include "mt_note.h"

typedef struct MtChord
{
  MtModifierType modifier;
  MtNote* notes[MAX_FRETS];
} MtChord;

MtChord* mt_chord_new(MtNote* notes[], MtModifierType modifier);

void mt_chord_free(MtChord* chord);

#endif
