#include "mt_chord.h"

#include <assert.h>

#include "mt_note.h"
#include "mt_alloc.h"

MtChord* mt_chord_new(MtQueue* notes, MtModifierType modifier)
{
  MtChord* chord = mt_alloc_object(MtChord);
  assert(chord != NULL);
  assert(notes != NULL);

  chord->notes = notes;
  chord->modifier = MT_MODIFIER_NONE;

  return chord;
}

void mt_chord_set_modifier(MtChord* chord, MtModifierType modifier)
{
  assert(chord != NULL);
  
  chord->modifier = modifier;
}

void mt_chord_free(MtChord* chord)
{
  assert(chord != NULL);

  // Fix this, dealloc the notes
  mt_free_object(chord);
}

