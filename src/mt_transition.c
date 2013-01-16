#include "mt_transition.h"

#include <assert.h>
#include <stdbool.h>
#include <string.h>

#include "mt_object.h"
#include "mt_alloc.h"
#include "mt_queue.h"
#include "mt_note.h"
#include "mt_chord.h"

// Private protos

// Get the ascii character for a transition
static char
mt_transition_get_char(MtTransition* transition);

// Set the transition character for a single note
static void mt_transition_set_note_string(MtTransition* transition, MtNote* note);

// Set the transition characters for a chord
static void mt_transition_set_chord_strings(MtTransition* transition, MtChord* chord);

// Public

MtTransition*
mt_transition_new(MtTransitionType type, MtObject* destination)
{
  MtTransition* transition = mt_alloc_object(MtTransition);
  assert(transition != NULL);

  assert(type != MT_TRANSITION_NONE);
  assert(destination != NULL);

  transition->type = type;

  // Initialize content to empty
  memset(transition->content, '\0', mt_conf.strings);

  // Calculate the transition strings
  switch (destination->type)
  {
    case MT_OBJ_NOTE:
      mt_transition_set_note_string(transition, destination->as.note);
      break;

    case MT_OBJ_CHORD:
      mt_transition_set_chord_strings(transition, destination->as.chord);
      break;

    default:
      assert(false);
      break;
  }

  return transition;
}

void
mt_transition_free(MtTransition* transition)
{
  assert(transition != NULL);
  mt_free_object(transition);
}

// Private

static char
mt_transition_get_char(MtTransition* transition)
{
  assert(transition != NULL);

  char value;
  switch (transition->type)
  {
    case MT_TRANSITION_BEND:
      value = 'b';
      break;

    case MT_TRANSITION_SLIDE_UP:
      value = '/';
      break;

    case MT_TRANSITION_SLIDE_DOWN:
      value = '\\';
      break;

    case MT_TRANSITION_HAMMER_ON:
      value = 'h';
      break;

    case MT_TRANSITION_PULL_OFF:
      value = 'p';
      break;

    default:
      assert(false);
      break;
  }

  return value;
}

static void
mt_transition_set_note_string(MtTransition* transition, MtNote* note)
{
  assert(transition != NULL);
  assert(note != NULL);

  char transition_char = mt_transition_get_char(transition);

  transition->content[note->string] = transition_char;
}

static void
mt_transition_set_chord_strings(MtTransition* transition, MtChord* chord)
{
  assert(transition != NULL);
  assert(chord != NULL);

  char transition_char = mt_transition_get_char(transition);

  mt_queue_each_val(chord->notes, {
    MtNote* note = (MtNote *) val;
    transition->content[note->string] = transition_char;
  });
}
