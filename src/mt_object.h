#ifndef MT_OBJECT_H
#define MT_OBJECT_H

#include "mt_type.h"

#include "mt_note.h"
#include "mt_chord.h"
#include "mt_transition.h"

typedef enum MtObjectType
{
  MT_OBJ_TYPE_NONE,
  MT_OBJ_TYPE_NOTE,
  MT_OBJ_TYPE_CHORD,
  MT_OBJ_TYPE_TRANSITION

} MtObjectType;

typedef struct MtObject
{
  MtObjectType type;

  union objectval
  {
    MtNote* note;
    MtChord* chord;
    MtTransition* transition;
  } objectval;

  int width;

} MtObject;

MtObject* mt_object_new(MtObjectType type, void* value);

void mt_object_free(MtObject* object);

#endif
