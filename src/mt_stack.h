#ifndef MT_STACK_H
#define MT_STACK_H

#include <stddef.h>

typedef struct MtStackNode
{
  void* value;
  struct MtStackNode* next;
} MtStackNode;

typedef struct MtStack
{
  MtStackNode* head;
  size_t size;
} MtStack;

MtStack* mt_stack_new();

void mt_stack_push(MtStack* stack, void* value);
void* mt_stack_pop(MtStack* stack);

void mt_stack_clear(MtStack* stack);
void mt_stack_free(MtStack* stack);

#endif
