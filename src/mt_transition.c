#include "mt_transition.h"

#include <stdio.h>
#include <assert.h>

#include "mt_object.h"
#include "mt_alloc.h"

MtTransition* mt_transition_new(MtTransitionType type, MtObject* start, MtObject* end)
{
  return NULL;
}

void mt_transition_free(MtTransition* transition)
{
  assert(transition != NULL);

  mt_free_object(transition);
}
