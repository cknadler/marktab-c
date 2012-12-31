#ifndef MT_STACK_H
#define MT_STACK_H

typedef struct MtStackNode
{
  void* value;
  struct MtStackNode* next;
} MtStackNode;

typedef struct MtStack
{
  MtStackNode* head;
} MtStack;

MtStack* mt_stack_new();

void mt_stack_push(MtStack* stack, void* value);
void* mt_stack_pop(MtStack* stack);

#endif
