#include "mtr.h"

#include <assert.h>

#include "mt_alloc.h"


// MtrScope
MtrScope* mtr_scope_new()
{
  MtrScope* scope = mt_alloc_object(MtrScope);
  assert(scope != NULL);

  // Sentinel value
  scope->last_note_used = 0;

  return scope;
}

void mtr_scope_free(MtrScope* scope)
{
  assert(scope != NULL);
  mt_free_object(scope);
}

// MtRuntime
void mtr_init()
{
  MTR.scopes = mt_stack_new();
  MTR.sections = mt_queue_new();

  mtr_scope_push();
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

void mtr_section_push(MtQueue* section)
{
  assert(section != NULL);

  mt_queue_enqueue(MTR.sections, section);
}

void mtr_destroy()
{
  mtr_scope_pop();
  mt_stack_free(MTR.scopes);
}
