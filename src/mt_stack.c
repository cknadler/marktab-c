#include "mt_stack.h"
#include <stdlib.h>
#include <assert.h>

// MtStackNode

static MtStackNode* mt_stack_node_new(void* value)
{
  MtStackNode* node = (MtStackNode *) malloc(sizeof(MtStackNode));
  assert(node != NULL);

  node->value = value;
  node->next = NULL;

  return node;
}

static void mt_stack_node_free(MtStackNode* node)
{
  assert(node != NULL);
  free(node);
}

// MtStack

MtStack* mt_stack_new()
{
  MtStack* stack = (MtStack *) malloc(sizeof(MtStack));
  assert(stack != NULL);

  stack->head = NULL;
  stack->size = 0;

  return stack;
}

void mt_stack_push(MtStack* stack, void* value)
{
  assert(stack != NULL);
  assert(value != NULL);

  MtStackNode* node = mt_stack_node_new(value);
  node->next = stack->head;
  stack->head = node;
  stack->size++;
}

void* mt_stack_pop(MtStack* stack)
{
  assert(stack != NULL);

  MtStackNode* node = stack->head;
  void* data = NULL;

  if (stack->head != NULL)
  {
    stack->size--;
    stack->head = stack->head->next;

    data = node->value;
    mt_stack_node_free(node);
  }

  assert(data != NULL);

  return data;
}

void* mt_stack_top(MtStack* stack)
{
  assert(stack != NULL);
  assert(stack->head->value != NULL);

  return stack->head->value;
}

void mt_stack_clear(MtStack* stack)
{
  assert(stack != NULL);
  MtStackNode* node = stack->head;
  MtStackNode* next;

  while (node != NULL)
  {
    next = node->next;
    mt_stack_node_free(node);
    node = next;
  }

  stack->head = NULL;
  stack->size = 0;
}

void mt_stack_free(MtStack* stack)
{
  assert(stack != NULL);
  mt_stack_clear(stack);
  free(stack);
}
