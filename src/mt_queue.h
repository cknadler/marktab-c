#ifndef MT_QUEUE_H
#define MT_QUEUE_H

#include <stddef.h>

typedef struct MtQueue
{
  int size;
  struct MtQueueNode* front;
  struct MtQueueNode* back;
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

#endif
