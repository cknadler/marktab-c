#include "mt_stack.h"
#include <stdlib.h>
#include <assert.h>

// MtStackNode
//

MtStackNode* mt_stack_node_new()
{
  MtStackNode* node = (MtStackNode *) malloc(sizeof(MtStackNode));
  assert(node != NULL);

  node->value = NULL;
  node->next = NULL;

  return node;
}

// MtStack

MtStack* mt_stack_new()
{
  MtStack* stack = (MtStack *) malloc(sizeof(MtStack));
  assert(stack != NULL);

  stack->head = NULL;

  return stack;
}

void mt_stack_push(MtStack* stack, void* value)
{
  assert(stack != NULL);
  assert(value != NULL);

  MtStackNode* node = mt_stack_node_new();
  node->value = value;
  node->next = stack->head;
  stack->head = node;
}

void* mt_stack_pop(MtStack* stack)
{
  assert(stack != NULL);

  MtStackNode* popped = stack->head;
  stack->head = popped->next;
  void* value = popped->value;
  free(popped);

  return value;
}
