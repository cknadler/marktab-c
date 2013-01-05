#include "mt_transition.h"

#include <stdio.h>
#include <assert.h>

#include "mt_object.h"
#include "mt_alloc.h"

MtTransition* mt_transition_new(MtTransitionType type, MtObject* destination)
{
  MtTransition* transition = mt_alloc_object(MtTransition);
  assert(transition != NULL);
  assert(type != MT_TRANSITION_NONE);

  assert(destination != NULL);
  assert((destination->type == MT_OBJ_NOTE) || 
         (destination->type == MT_OBJ_CHORD));

  transition->type = type;
  transition->destination = destination;

  return transition;
}

void mt_transition_free(MtTransition* transition)
{
  assert(transition != NULL);
  mt_free_object(transition);
}
