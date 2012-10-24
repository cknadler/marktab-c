#include "mt_note.h"

#include "mt_alloc.h"
#include <assert.h>

MtNote* mt_note_new(int string, int fret, MtModifierType modifier)
{
  MtNote* note = mt_alloc_object(MtNote);
  assert(note != NULL);

  note->type = MT_NOTE_TYPE_NOTE;

  note->string = string;
  note->fret = fret;
  note->modifier = modifier;

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

void mt_note_free(MtNote* note)
{
  mt_free_object(note);
}
