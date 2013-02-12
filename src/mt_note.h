#ifndef MT_NOTE_H
#define MT_NOTE_H

#include "mt_modifier.h"

typedef enum MtNoteType
{
  MT_NOTE_NONE,
  MT_NOTE_NOTE,
  MT_NOTE_MUTE
} MtNoteType;

typedef struct MtNote
{
  MtNoteType type;
  int string;
  int fret;
  MtModifier modifier;
  int size;
} MtNote;

// Constructors
MtNote* mt_note_new(int string, int fret);
MtNote* mt_note_new_muted(int string);
MtNote* mt_note_new_without_fret(int string);

// Setters
void mt_note_set_modifier(MtNote* note, MtModifier modifier);
void mt_note_set_fret(MtNote* note, int fret);
void mt_note_set_muted(MtNote* note);

// Destructors
void mt_note_free(MtNote* note);

#endif
