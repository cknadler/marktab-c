#ifndef MTR_H
#define MTR_H

#include "mt_stack.h"
#include "mt_queue.h"
#include "mt_object.h"
#include "mt_hash.h"

typedef struct MtrScope
{
  int last_note_string;
  MtQueue* current_object_queue;
} MtrScope;

typedef struct MtRuntime
{
  MtStack* scopes;
  MtQueue* sections;
  MtHash* symbol_table;
} MtRuntime;

MtRuntime MTR;

// Initialize the marktab runtime
void mtr_init();

// Returns a pointer to the current object queue of the topmost scope
MtQueue* mtr_current_object_queue();

// Returns the last note used in the current scope of the runtime
int mtr_last_note_string();

// Sets the last note string for the current scope
void mtr_set_last_note_string(int string);

// Enqueue an object in the current scopes object queue
void mtr_current_object_queue_enqueue(MtObject* object);

// Push a new scope on the stack
void mtr_scope_push();

// Pop the current scope off the stack
void mtr_scope_pop();

// Push a parsed output section into the runtime's output queue
void mtr_section_push();

// Destroy the marktab runtime
// Clears all runtime memory
void mtr_destroy();

#endif
