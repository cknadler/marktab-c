#ifndef MT_QUEUE_H
#define MT_QUEUE_H

#include <stddef.h>

typedef struct MtQueueNode
{
  void* value;
  struct MtQueueNode* next;
} MtQueueNode;

typedef struct MtQueue
{
  int size;
  MtQueueNode* front;
  MtQueueNode* back;
} MtQueue;

// Creates a new empty queue
MtQueue* mt_queue_new();

// Adds element to back of queue
void mt_queue_enqueue(MtQueue* queue, void* value);

// Pops element from front of queue
void* mt_queue_dequeue(MtQueue* queue);

// Clears queue data
void mt_queue_clear(MtQueue* queue);

// Frees queue and all associated resources
void mt_queue_free(MtQueue* queue);

// Iterate over each value of the queue and execute a block
// Loads the values in a local variable `val`
// Note that this doesn't dequeue the values
#define mt_queue_each_val(self, block) { \
    MtQueueNode* node = NULL; \
    int queue_pos; \
    void* val; \
    for (queue_pos = 0; queue_pos < self->size; ++queue_pos) { \
      if (queue_pos == 0) node = self->front; else node = node->next; \
      val = node->value; \
      block; \
    } \
  }

// Dequeue each value in the queue
// Loads the values in a local variable `val`
#define mt_queue_dequeue_each_val(self, block) { \
    void* val; \
    while (self->size > 0) { \
      val = mt_queue_dequeue(self); \
      block; \
    } \
  } \

#endif // MT_QUEUE_H
