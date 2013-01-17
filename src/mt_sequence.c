#include "mt_sequence.h"

#include <assert.h>

#include "mt_alloc.h"
#include "mt_object.h"

MtSequence* mt_sequence_new(MtQueue* objects)
{
  MtSequence* sequence = mt_alloc_object(MtSequence);
  assert(sequence != NULL);
  assert(objects != NULL);

  sequence->objects = objects;

  sequence->modifier = MT_MODIFIER_NONE;
  
  return sequence;
}

void mt_sequence_set_modifier(MtSequence* sequence, MtModifier modifier)
{
  assert(sequence != NULL);
  sequence->modifier = modifier;
}

void mt_sequence_free(MtSequence* sequence)
{
  assert(sequence != NULL);
  // TODO: This will leak memory, fix
  mt_free_object(sequence);
}
