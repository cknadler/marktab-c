#include "mtr.h"

#include <assert.h>

#include "mt_alloc.h"

//
// MtrScope
//
MtrScope* mtr_scope_new()
{
  MtrScope* scope = mt_alloc_object(MtrScope);
  assert(scope != NULL);

  scope->last_note_string = 0;
  scope->current_object_queue = mt_queue_new();

  return scope;
}

void mtr_scope_free(MtrScope* scope)
{
  assert(scope != NULL);
  scope->current_object_queue = NULL;
  mt_free_object(scope);
}

//
// MtRuntime
//
void mtr_init()
{
  MTR.scopes = mt_stack_new();
  MTR.sections = mt_queue_new();
  MTR.symbol_table = mt_hash_new();
}

MtQueue* mtr_current_object_queue()
{
  MtrScope* scope = mt_stack_top(MTR.scopes);
  return scope->current_object_queue;
}

int mtr_last_note_string()
{
  MtrScope* scope = mt_stack_top(MTR.scopes);
  return scope->last_note_string;
}

void mtr_set_last_note_string(int string)
{
  MtrScope* scope = mt_stack_top(MTR.scopes);
  scope->last_note_string = string;
}

void mtr_current_object_queue_enqueue(MtObject* object)
{
  assert(object != NULL);

  MtrScope* scope = mt_stack_top(MTR.scopes);
  mt_queue_enqueue(scope->current_object_queue, object);
}

void mtr_scope_push()
{
  mt_stack_push(MTR.scopes, mtr_scope_new());
}

void mtr_scope_pop()
{
  MtrScope* scope = mt_stack_pop(MTR.scopes);
  assert(scope != NULL);

  mtr_scope_free(scope);
}

void mtr_section_push()
{
  mt_queue_enqueue(MTR.sections, mtr_current_object_queue());
}

void mtr_destroy()
{
  mtr_scope_pop();
  mt_stack_free(MTR.scopes);
  mt_hash_free(MTR.symbol_table);
}
