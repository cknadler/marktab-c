#ifndef MT_BUFFER
#define MT_BUFFER

#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>

// Char array buffer
typedef struct MtBuffer
{
  uint8_t* data; // Actual data
  size_t size; // Size of data
  size_t asize; // Allocated size of buffer
  size_t unit; // Reallocation unit size
} MtBuffer;

// Allocate a new buffer with a reallocation size of unit
MtBuffer* mt_buffer_new(size_t unit);

// Append raw data to a buffer
void mt_buffer_put(MtBuffer* buf, void* data, size_t len);

// Append a null terminated string to a buffer
void mt_buffer_puts(MtBuffer* buf, const char* string);

// Append a single character to a buffer
void mt_buffer_putc(MtBuffer* buf, int c);

// Clear the internal data of a buffer
void mt_buffer_clear(MtBuffer*);

// Deallocate a buffer
void mt_buffer_free(MtBuffer*);

#endif // MT_BUFFER
