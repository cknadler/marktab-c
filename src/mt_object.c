#include "mt_object.h"

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "mt_alloc.h"

MtObject* mt_object_new(MtObjectType type, void* value)
{
  MtObject* object = mt_alloc_object(MtObject);
  assert(object != NULL);

  switch (type)
  {
    case MT_OBJ_TYPE_NOTE: 
      object->as.note = value;
      break;
      
    case MT_OBJ_TYPE_CHORD:
      object->as.chord = value;
      break;

    case MT_OBJ_TYPE_TRANSITION:
      object->as.transition = value;
      break;

    default:
      // MtObject should be constructed with a valid type
      assert(false);
      break;
  }

  object->type = type;

  return object;
}

void mt_object_free(MtObject* object)
{
  assert(object != NULL);

  switch (object->type)
  {
    case MT_OBJ_TYPE_NOTE:
      mt_note_free(object->as.note);
      break;

    case MT_OBJ_TYPE_CHORD:
      mt_chord_free(object->as.chord);
      break;

    case MT_OBJ_TYPE_TRANSITION:
      mt_transition_free(object->as.transition);
      break;

    default:
      // MtObject should always have a valid type
      assert(false);
      break;
  }

  mt_free_object(object);
}
