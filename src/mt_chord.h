#ifndef MT_CHORD_H
#define MT_CHORD_H

#include "mt_type.h"
#include "mt_queue.h"

typedef struct MtChord
{
  MtQueue* notes;
  MtModifierType modifier;
} MtChord;

MtChord* mt_chord_new(MtQueue* notes, MtModifierType modifier);

void mt_chord_free(MtChord* chord);

#endif
