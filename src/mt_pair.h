#ifndef MT_PAIR_H
#define MT_PAIR_H

#include "mt_string.h"

// Generic data type for storing key value pairs
typedef struct MtPair
{
  MtString* key;
  void* value;
} MtPair;

// Create a new pair
// Key is managed by the structure
// The value is not
MtPair* mt_pair_new(MtString* key, void* value);

// Free a pair and its associated key
void mt_pair_free(MtPair* pair);

#endif // MT_PAIR_H
