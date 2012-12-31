#include "mt_note.h"

#include "mt_alloc.h"
#include <assert.h>

MtNote* mt_note_new(int string, int fret)
{
  MtNote* note = mt_alloc_object(MtNote);
  assert(note != NULL);

  note->type = MT_NOTE_TYPE_NOTE;

  note->string = string;
  note->fret = fret;
  note->modifier = MT_MODIFIER_NONE;

  return note;
}

MtNote* mt_note_new_muted(int string)
{
  MtNote* note = mt_alloc_object(MtNote);
  assert(note != NULL);

  note->type = MT_NOTE_TYPE_MUTE;

  note->string = string;

  return note;
}

void mt_note_set_modifier(MtNote* note, MtModifierType type)
{
  assert(note != NULL);

  note->modifier = type;
}

void mt_note_free(MtNote* note)
{
  assert(note != NULL);

  mt_free_object(note);
}
