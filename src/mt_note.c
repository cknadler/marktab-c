#include "mt_note.h"

#include <assert.h>

#include "mt_alloc.h"
#include "mt_math.h"

MtNote* mt_note_new_without_fret(int string)
{
  MtNote* note = mt_alloc_object(MtNote);
  assert(note != NULL);
  assert(string > 0);

  note->type = MT_NOTE_NONE;

  note->string = string - 1;
  note->modifier = MT_MODIFIER_NONE;

  // This is a partial note, size is currently 0
  note->size = 0;

  return note;
}

MtNote* mt_note_new(int string, int fret)
{
  MtNote* note = mt_alloc_object(MtNote);
  assert(note != NULL);
  assert(string > 0);
  assert(fret >= 0);

  note->type = MT_NOTE_NOTE;

  note->string = string - 1;
  note->fret = fret;
  note->modifier = MT_MODIFIER_NONE;

  // Calculate the size (width) of the printed note
  note->size = digits(fret);

  return note;
}

MtNote* mt_note_new_muted(int string)
{
  MtNote* note = mt_alloc_object(MtNote);
  assert(note != NULL);

  note->type = MT_NOTE_MUTE;

  note->string = string - 1;

  note->size = 1;

  return note;
}

void mt_note_set_modifier(MtNote* note, MtModifier modifier)
{
  assert(note != NULL);
  note->modifier = modifier;
}

void mt_note_set_fret(MtNote* note, int fret)
{
  assert(note != NULL);
  assert(note->type == MT_NOTE_NONE);

  note->type = MT_NOTE_NOTE;
  note->fret = fret;
  note->size = digits(fret);
}

void mt_note_set_muted(MtNote* note)
{
  assert(note != NULL);
  assert(note->type == MT_NOTE_NONE);

  note->type = MT_NOTE_MUTE;
  note->size = 1;
}

void mt_note_free(MtNote* note)
{
  assert(note != NULL);

  mt_free_object(note);
}
