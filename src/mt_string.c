#include <assert.h>
#include <string.h>

#include "mt_string.h"
#include "mt_alloc.h"

// Get length of utf8-encoded string
static size_t utf8_length(const char* utf8_data) {
   size_t i = 0, j = 0;

   while (utf8_data[i]) {

     if ((utf8_data[i] & 0xC0) != 0x80) j++;
     i++;
   }
   return j;
}

MtString* mt_string_new()
{
  MtString* const string = mt_alloc_object(MtString);
  assert(string != NULL);

  string->utf8_data = NULL;
  string->size = 0;
  string->hash = 0;

  return string;
}

MtString* mt_string_new_from_string(const MtString* string)
{
  assert(string != NULL);

  MtString* copy = mt_string_new();
  mt_string_copy(copy, string);

  return copy;
}

MtString* mt_string_new_from_integer(int integer)
{
  // create a buffer to store the integer
  MtString* string = mt_string_new_from_utf8("               ");
  sprintf(string->utf8_data,"%d", integer);
  string->length = utf8_length(string->utf8_data);
  return string;
}

MtString* mt_string_new_from_real(double real)
{
  MtString* string = mt_string_new_from_utf8("               ");
  sprintf(string->utf8_data, "%lg", real);
  string->length = utf8_length(string->utf8_data);
  return string;
}

MtString* mt_string_new_from_stream(FILE* stream)
{
  // initialize an 8 character input buffer
  MtString* string = mt_string_new_from_utf8("       ");
  size_t input = 0;
  int c;

  while (true)
  {
    c = fgetc(stdin);

    if (c == EOF || c == '\n')
      break;

    // if we are about to overflow
    if (input == string->size)
    {
      // double the buffer
      string->size *= 2;
      string->utf8_data = realloc(string->utf8_data, string->size);
    }

    string->utf8_data[input] = c;
    input++;
  }

  // write a null character
  string->utf8_data[input] = '\0';

  string->length = utf8_length(string->utf8_data);

  return string;
}

MtString* mt_string_new_from_utf8(const char* utf8_data)
{
  assert(utf8_data != NULL);

  MtString* string = mt_string_new();
  mt_string_set_utf8(string, utf8_data);

  return string;
} 

void mt_string_set_utf8(MtString* string, const char* utf8_data)
{
  assert(string != NULL);
  assert(utf8_data != NULL);

  size_t size = strlen(utf8_data);
  
  // The input string must be null-terminated
  assert(utf8_data[size] == '\0');

  // Resize the string to accomidate new data
  if (string->size < size + 1)
    string->utf8_data = realloc(string->utf8_data, size + 1);

  // This used to only copy size, but that caused a horrible memory leak
  string->size = size + 1;
  string->length = utf8_length(utf8_data);

  memcpy(string->utf8_data, utf8_data, size + 1);

  // The string must be NULL terminated by now
  assert(string->utf8_data[size] == '\0');

  // The hash is invalid
  string->hash = 0;
}

void mt_string_cat(MtString* string, const MtString* src)
{
  assert(string != NULL);
  assert(src != NULL);

  // For now this works
  // Later this can be sped up by not relying on this function.
  mt_string_cat_utf8(string, src->utf8_data);

  // The hash is invalid
  string->hash = 0;
}

void mt_string_cat_utf8(MtString* string, const char* utf8_data)
{
  assert(string != NULL);
  assert(utf8_data != NULL);

  size_t size = strlen(utf8_data);

  // The input string must be null-terminated
  assert(utf8_data[size] == '\0');

  // Resize the string to accomidate new data
  if (string->size < string->size + size + 1)
    string->utf8_data = realloc(string->utf8_data, string->size + size + 1);

  string->length += utf8_length(utf8_data);

  memcpy(string->utf8_data + string->size, utf8_data, size + 1);

  string->size = string->size + size;

  // The string must be NULL terminated by now
  // minus 1 because string->size includes null
  assert(string->utf8_data[string->size] == '\0');

  // The hash is invalid
  string->hash = 0;
}
  
void mt_string_copy(MtString* string, const MtString* src)
{
  assert(string != NULL);
  assert(src != NULL);

  mt_string_set_utf8(string, src->utf8_data);

  // Copy the hash from the src string
  // >_<'
  string->hash = src->hash;
}

int mt_string_compare(const MtString* string1, const MtString* string2)
{
  assert(string1 != NULL);
  assert(string2 != NULL);

  // Since we are doing a strncmp, we need to make sure we pick the larger
  // of the two strings as the size
  size_t size = (string1->length > string2->length)?
    string1->length : string2->length;

  return strncmp(string1->utf8_data, string2->utf8_data, size);
}

int mt_string_compare_utf8(MtString* string, const char* utf8_data)
{
  assert(string != NULL);
  assert(utf8_data != NULL);

  // Since we are doing a strncmp, we need to make sure we pick the larger
  // of the two strings as the size
  size_t ulen = utf8_length(utf8_data);
  size_t size = (string->length > ulen)?
    string->length : ulen;

  return strncmp(string->utf8_data, utf8_data, size);
}

void mt_string_clear(MtString* string)
{
  assert(string != NULL);

  free(string->utf8_data);

  string->utf8_data = NULL;
  string->size = 0;
  string->length = 0;

  // The hash is invalid
  string->hash = 0;
}

const char* mt_string_get_utf8(MtString* string)
{
  assert(string != NULL);

  return string->utf8_data;
}

size_t mt_string_get_size(const MtString* string)
{
  assert(string != NULL);

  return string->size;
}

size_t mt_string_get_length(const MtString* string)
{
  assert(string != NULL);

  return string->length;
}

void mt_string_free(MtString* string)
{
  assert(string != NULL);

  mt_string_clear(string);
  mt_free_object(string);
}

