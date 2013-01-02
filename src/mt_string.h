#ifndef MT_STRING_H
#define MT_STRING_H

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

/**
@struct MtString A utf8-based string implementation.

Since utf-8 internally can use anywhere between 1 and 4 bytes per character
MtString acts as a wrapper for utf-8 data.

@authors Scott LaVigne
@date 9/7/2012
*/
typedef struct MtString
{
  char* utf8_data;
  size_t size;
  size_t length;
  size_t hash;

} MtString;

/**
Allocate a new MtString.

@retval MtString* A new MtString.
@brief Allocates a new empty MtString.
*/
MtString* mt_string_new();

/**
Allocate a copy of another MtString.

@param string The string to make a copy of.
@retval MtString* A newly allocated copy of string.
*/
MtString* mt_string_new_from_string(const MtString* string);

/**
Allocate a string representation of an integer.

@param integer The integer value to copy.
@retval MtString* A newly allocated string.
*/
MtString* mt_string_new_from_integer(int integer);

/**
Allocate a string representation of a real number.

@param real The real value to copy.
@retval MtString* A newly allocated string.
*/
MtString* mt_string_new_from_real(double real);


MtString* mt_string_new_from_stream(FILE* stream);

/**
Allocate a new MtString using utf8 data.

@param utf8_data The null-terminated utf8 data to copy into the MtString.
@retval MtString* A new MtString.
@brief Allocates a new MtString, with an internal copy of a utf8 string.
*/
MtString* mt_string_new_from_utf8(const char* utf8_data);

/**
Sets the utf8 data inside a MtString.

@param string The MtString to modify.
@param utf8_data The null-terminated utf8 data to copy into the MtString.
@brief Modifies string with a copy of a utf8 string.
*/
void mt_string_set_utf8(MtString* string, const char* utf8_data);

/**
Concatinate a MtString.

@param string The MtString to add data to.
@param src The MtString to append.
@brief The string is modified in-place only reallocating internal data if needed.
*/
void mt_string_cat(MtString* string, const MtString* src);

/**
Concatinate a MtString with utf8 data.

@param string The MtString to add data to.
@param utf8_data The null-terminated utf8 string to append.
@brief The string is modified in-place only reallocating internal data if needed.
*/
void mt_string_cat_utf8(MtString* string, const char* utf8_data);

/**
Turns a MtString into an exact copy of another.

@param string The MtString to modify.
@param src The MtString to copy.
*/
void mt_string_copy(MtString* string, const MtString* src);

/**
Compares two MtStrings.

@param string1 A MtString object.
@param string2 A second MtString object to compare with.
@retval int -1 If string1<string2 0 If string1==string2 1 If string1>string2
*/
int mt_string_compare(const MtString* string1, const MtString* string2);

/**
Compares a MtString with utf8 data.

@param string A MtString object.
@param utf8_data A null-terminated utf8 string to compare.
@retval int -1 If string<utf8 0 If string==utf8 1 If string>utf8
*/
int mt_string_compare_utf8(MtString* string, const char* utf8_data);

/**
Erase the contents of a MtString

@post The internal utf8 data is freed.

@param string The MtString to clear.
*/
void mt_string_clear(MtString* string);

/**
Get the raw utf8_data within a MtString

@param string The MtString object.
@retval uint8_t* A pointer to the utf8 data within the string
*/
const char* mt_string_get_utf8(MtString* string);

/**
Get the number of utf8 bytes in a MtString.
This does not include the null byte.

@param string The MtString object.
@retval size_t The number of bytes in the string.
*/
size_t mt_string_get_size(const MtString* string);

/**
Get the number of characters in the MtString.
This does not include the null character.

@param string The MtString object.
@retval size_t The number of characters in the string.
*/
size_t mt_string_get_length(const MtString* string);

/**
Free a MtString and releases any allocated resources it has.

@param string The MtString to free.
*/
void mt_string_free(MtString* string);

#endif // MT_STRING_H

