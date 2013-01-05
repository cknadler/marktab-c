#ifndef MTR_H
#define MTR_H

#include "mt_stack.h"
#include "mt_queue.h"

typedef struct MtrScope
{
  int last_note_used;
} MtrScope;

typedef struct MtRuntime
{
  MtStack* scopes;
  MtQueue* sections;
} MtRuntime;

MtRuntime MTR;

// Initialize the marktab runtime
void mtr_init();

// Push a new scope on the stack
void mtr_scope_push();

// Pop the current scope off the stack
void mtr_scope_pop();

// Push a parsed output section into the runtime's output queue
void mtr_section_push(MtQueue* section);

// Destroy the marktab runtime
// Clears all runtime memory
void mtr_destroy();

#endif
