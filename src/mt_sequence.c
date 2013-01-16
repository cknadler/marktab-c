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

// Recursively set the modifier of all contained objects
void mt_sequence_set_modifier(MtSequence* sequence, MtModifier modifier)
{
  if (sequence->modifier != modifier)
  {
    mt_queue_each_val(sequence->objects, {

      MtObject* object = (MtObject *) val;
      switch (object->type)
      {
        case MT_OBJ_NOTE:
          mt_note_set_modifier(object->as.note, modifier);
          break;

        case MT_OBJ_CHORD:
          mt_chord_set_modifier(object->as.chord, modifier);
          break;

        case MT_OBJ_SEQUENCE:
          mt_sequence_set_modifier(object->as.sequence, modifier);
          break;

        default:
          // Do nothing yo
          break;
      }
    });
  }
}

void mt_sequence_free(MtSequence* sequence)
{
  assert(sequence != NULL);
  // TODO: This will leak memory, fix
  mt_free_object(sequence);
}
