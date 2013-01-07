#ifndef MT_SEQUENCE_H
#define MT_SEQUENCE_H

#include "mt_type.h"
#include "mt_queue.h"

typedef struct MtSequence
{
  MtModifierType modifier;
  MtQueue* objects;
} MtSequence;

MtSequence* mt_sequence_new(MtQueue* objects);
void mt_sequence_set_modifier(MtSequence* sequence, MtModifierType modifier);

void mt_sequence_free(MtSequence* sequence);

#endif
