#ifndef MT_NOTE_H
#define MT_NOTE_H

#include "mt_type.h"

typedef struct MtNote
{
  MtModifierType modifier;
  int string;
  int fret; 
} MtNote;

MtNote* mt_note_new(int string, int fret, ...);

#endif
