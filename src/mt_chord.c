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
    if (note->size > chord->size) chord->size = note->size;
  });

  return chord;
}

void mt_chord_set_modifier(MtChord* chord, MtModifier modifier)
{
  assert(chord != NULL);
  
  if((chord->modifier == MT_MODIFIER_NONE) && (modifier != MT_MODIFIER_NONE))
  {
    chord->size++;
  }

  // Set the modifier of all contained notes
  mt_queue_each_val(chord->notes, {
    MtNote* note = (MtNote *) val;
    mt_note_set_modifier(note, modifier); 
  });

  chord->modifier = modifier;
}

void mt_chord_free(MtChord* chord)
{
  assert(chord != NULL);
  mt_queue_free(chord->notes);
  mt_free_object(chord);
}

