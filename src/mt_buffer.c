#include "mt_buffer.h"

#include <stdlib.h>
#include <assert.h>

#define MT_BUFFER_MAX_SIZE (1024 * 1024 * 16) //16mb

MtBuffer* mt_buffer_new(size_t unit)
{
  MtBuffer* buf = (MtBuffer*) malloc(sizeof(MtBuffer));
  assert(buf != NULL);

  buf->data = 0;
  buf->size = buf->asize = 0;
  buf->unit = unit;

  return buf;
}

void mt_buffer_put(MtBuffer* buf, void* data, size_t len)
{
  assert(buf != NULL);
  assert(data != NULL);
}

void mt_buffer_puts(MtBuffer* buf, const char* string)
{
  assert(buf != NULL);
  assert(string != NULL);
}

void mt_buffer_putc(MtBuffer* buf, int c)
{
  assert(buf != NULL);
}

void mt_buffer_clear(MtBuffer* buf)
{
  assert(buf != NULL);

  free(buf->data);
  buf->data = NULL;
  buf->size = buf->asize = 0;
}

void mt_buffer_free(MtBuffer* buf)
{
  assert(buf != NULL);

  mt_buffer_clear(buf);
  free(buf);
}
