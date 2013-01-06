#ifndef MT_CHORD_H
#define MT_CHORD_H

#include "mt_type.h"
#include "mt_queue.h"

typedef struct MtChord
{
  MtQueue* notes;
  MtModifierType modifier;
  int size;
} MtChord;

// Constructors
MtChord* mt_chord_new(MtQueue* notes);

// Setters
void mt_chord_set_modifier(MtChord* chord, MtModifierType type);

// Destructors
void mt_chord_free(MtChord* chord);

#endif // MT_CHORD_H
