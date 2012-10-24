#ifndef MT_NOTE_H
#define MT_NOTE_H

#include "mt_type.h"

#include <stdbool.h>

typedef enum MtNoteType
{
  MT_NOTE_TYPE_NOTE,
  MT_NOTE_TYPE_MUTE

} MtNoteType;

typedef struct MtNote
{
  MtNoteType type;
  int string;
  int fret;
  MtModifierType modifier;

} MtNote;

MtNote* mt_note_new(int string, int fret, MtModifierType modifier);

MtNote* mt_note_new_muted(int string);

void mt_note_free(MtNote* note);

#endif
