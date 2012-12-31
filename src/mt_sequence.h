#ifndef MT_SEQUENCE_H
#define MT_SEQUENCE_H

#include "mt_type.h"
#include "mt_queue.h"

typedef struct MtSequence
{
  MtModifierType modifier;
  MtQueue* objects;
} MtSequence;

MtSequence* mt_sequence_new();
void mt_sequence_add_object(MtSequence* sequence, void* object);
void mt_sequence_set_modifier(MtSequence* sequence, MtModifierType modifier);

void mt_sequence_free(MtSequence* sequence);

#endif
