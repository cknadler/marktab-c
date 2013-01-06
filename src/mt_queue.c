#include "mt_queue.h"

#include <assert.h>
#include "mt_alloc.h"

/*
 * MtQueueNode
 */

// Creates a new node
MtQueueNode* mt_queue_node_new(void* value);

// Frees the node and all associated resources
void mt_queue_node_free(MtQueueNode* node);

MtQueueNode* mt_queue_node_new(void* value)
{
  MtQueueNode* node = mt_alloc_object(MtQueueNode);  
  assert(node != NULL);

  node->value = value;

  return node;
}

void mt_queue_node_free(MtQueueNode* node)
{
  assert(node != NULL);
  
  node->value = NULL;
  node->next = NULL;

  mt_free_object(node);
}


/*
 * MtQueue
 */

MtQueue* mt_queue_new()
{
  MtQueue* queue = mt_alloc_object(MtQueue);
  assert(queue != NULL);

  queue->size = 0;
  queue->front = NULL;
  queue->back = NULL;

  return queue;
}

void mt_queue_enqueue(MtQueue* queue, void* value)
{
  assert(queue != NULL);
  
  MtQueueNode* new_node = mt_queue_node_new(value);

  if (queue->size > 0)
  {
    queue->back->next = new_node;
    queue->back = new_node;
  } 
  else 
  {
    queue->front = new_node;
    queue->back = new_node;
  }

  queue->size++;
}

void* mt_queue_dequeue(MtQueue* queue)
{
  assert(queue != NULL);

  if (queue->size > 0)
  {
    MtQueueNode* front_node = queue->front;
    void* value = front_node->value;

    queue->front = front_node->next;

    mt_queue_node_free(front_node);

    --queue->size;

    return value;
  }

  // Case of an empty queue
  return NULL;
}

void mt_queue_clear(MtQueue* queue)
{
  assert(queue != NULL);

  // TODO: Fix memory deallocation of queue
  // This will leak, all nodes need to be freed
  queue->size = 0;
  queue->front = NULL;
  queue->back = NULL;
}

void mt_queue_free(MtQueue* queue)
{
  assert(queue != NULL);
  mt_queue_clear(queue);
  mt_free_object(queue);
}
