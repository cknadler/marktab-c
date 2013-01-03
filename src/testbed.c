#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "mt_stack.h"

typedef struct Object
{
  int value;
} Object;

Object* object_new(int value)
{
  Object* object = (Object *) malloc(sizeof(Object));
  assert(object != NULL);

  object->value = value;

  return object;
}

int main() 
{
  // For testing data structures

  return 0;
}
