#ifndef MT_TRANSITION_H
#define MT_TRANSITION_H

#include "mt_type.h"
#include "mt_config.h"

struct MtObject;

typedef enum MtTransitionType
{
  MT_TRANSITION_NONE,
  MT_TRANSITION_BEND,
  MT_TRANSITION_SLIDE_UP,
  MT_TRANSITION_SLIDE_DOWN,
  MT_TRANSITION_HAMMER_ON,
  MT_TRANSITION_PULL_OFF
} MtTransitionType;

typedef struct MtTransition
{
  MtTransitionType type;
  char content[MT_CONFIG_MAX_STRINGS];
} MtTransition;

MtTransition* mt_transition_new(MtTransitionType type, struct MtObject* destination);

void mt_transition_free(MtTransition* transition);

#endif // MT_TRANSITION_H
