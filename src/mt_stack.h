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

// Push an element onto the stack
void mt_stack_push(MtStack* stack, void* value);

// Pops a value off the stack
void* mt_stack_pop(MtStack* stack);

// Returns the value of the top element of the stack
// Doesn't pop it
void* mt_stack_top(MtStack* stack);

// Clear all elements in the stack
void mt_stack_clear(MtStack* stack);

// Free the stack
// Clears, before it frees
void mt_stack_free(MtStack* stack);

#endif
