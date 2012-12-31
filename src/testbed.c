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
  return object;
}

int main() 
{

  Object* object1 = object_new(1);
  Object* object2 = object_new(2);
  
  MtStack* stack = mt_stack_new();

  mt_stack_push(stack, object1);
  mt_stack_push(stack, object2);

  object2 = mt_stack_pop(stack);
  object1 = mt_stack_pop(stack);

  printf("%d", object1->value);
  printf("%d", object2->value);

  return 0;
}
