#include "mt_queue.h"

#include <assert.h>
#include <stdlib.h>

/*
 * MtQueueNode
 */

MtQueueNode* mt_queue_node_new(void* value)
{
  MtQueueNode* node = (MtQueueNode *) malloc(sizeof(MtQueueNode));
  assert(node != NULL);

  node->value = value;
  node->next = NULL;

  return node;
}

void mt_queue_node_free(MtQueueNode* node)
{
  assert(node != NULL);

  node->value = NULL;
  node->next = NULL;

  free(node);
}


/*
 * MtQueue
 */

MtQueue* mt_queue_new()
{
  MtQueue* queue = (MtQueue *) malloc(sizeof(MtQueue));
  assert(queue != NULL);

  queue->length = 0;
  queue->front = NULL;
  queue->back = NULL;

  return queue;
}

void mt_queue_enqueue(MtQueue* queue, void* value)
{
  assert(queue != NULL);
  assert(value != NULL);

  MtQueueNode* node = mt_queue_node_new(value);

  if (queue->length > 0)
  {
    queue->back->next = node;
    queue->back = node;
  }
  else
  {
    queue->front = node;
    queue->back = node;
  }

  ++queue->length;
}

void* mt_queue_dequeue(MtQueue* queue)
{
  assert(queue != NULL);

  if (queue->length > 0)
  {
    MtQueueNode* front_node = queue->front;
    void* value = front_node->value;

    queue->front = front_node->next;

    mt_queue_node_free(front_node);

    --queue->length;

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
  queue->length = 0;
  queue->front = NULL;
  queue->back = NULL;
}

void mt_queue_free(MtQueue* queue)
{
  assert(queue != NULL);
  mt_queue_clear(queue);
  free(queue);
}
