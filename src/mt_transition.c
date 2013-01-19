#include "mt_transition.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "mt_object.h"
#include "mt_alloc.h"
#include "mt_queue.h"
#include "mt_note.h"
#include "mt_chord.h"

MtTransition*
mt_transition_new(MtTransitionType type, MtObject* destination)
{
  MtTransition* transition = mt_alloc_object(MtTransition);
  assert(transition != NULL);

  assert(type != MT_TRANSITION_NONE);
  assert(destination != NULL);

  transition->type = type;
  transition->destination = destination;

  return transition;
}

void
mt_transition_free(MtTransition* transition)
{
  assert(transition != NULL);
  mt_free_object(transition);
}
