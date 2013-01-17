#include "mt_chord.h"

#include <assert.h>

#include "mt_note.h"
#include "mt_alloc.h"
#include "mt_math.h"

MtChord* mt_chord_new(MtQueue* notes)
{
  MtChord* chord = mt_alloc_object(MtChord);
  assert(chord != NULL);
  assert(notes != NULL);

  // TODO: Check for duplicate strings

  chord->notes = notes;
  chord->modifier = MT_MODIFIER_NONE;

  // Calculate size
  chord->size = 0;
  mt_queue_each_val(notes, {
    MtNote* note = (MtNote *) val;
    if (note->size > chord->size)
      chord->size = note->size;
  });

  return chord;
}

void mt_chord_set_modifier(MtChord* chord, MtModifier modifier)
{
  assert(chord != NULL);
  chord->modifier = modifier;
}

void mt_chord_free(MtChord* chord)
{
  assert(chord != NULL);
  mt_queue_free(chord->notes);
  mt_free_object(chord);
}

