#ifndef MT_OBJECT_H
#define MT_OBJECT_H

#include "mt_type.h"

#include "mt_note.h"
#include "mt_chord.h"
#include "mt_transition.h"
#include "mt_sequence.h"

typedef enum MtObjectType
{
  MT_OBJ_TYPE_NONE,
  MT_OBJ_TYPE_NOTE,
  MT_OBJ_TYPE_CHORD,
  MT_OBJ_TYPE_TRANSITION,
  MT_OBJ_TYPE_SEQUENCE,
  MT_OBJ_TYPE_REST
} MtObjectType;

typedef struct MtObject
{
  MtObjectType type;

  union asvalue
  {
    MtNote* note;
    MtChord* chord;
    MtTransition* transition;
    MtSequence* sequence;
  } as;

} MtObject;

MtObject* mt_object_new(MtObjectType type, void* value);
MtObject* mt_object_new_rest();

void mt_object_free(MtObject* object);

#endif
