#ifndef MT_HASH_H
#define MT_HASH_H

#include <stdbool.h>
#include <stddef.h>

#include "mt_string.h"
#include "mt_pair.h"

typedef struct MtHash
{
  struct MtHashElement* buckets;
  size_t size;
  size_t length;

} MtHash;

MtHash* mt_hash_new();

MtPair* mt_hash_search(MtHash* hash, MtString* key);

void mt_hash_insert(MtHash* hash, MtString* key, void* value);

void mt_hash_move_pair(MtHash* hash, MtPair* pair);

void mt_hash_traverse(MtHash* hash, bool(*fn)(MtPair*, void*), void* data);

void mt_hash_remove(MtHash* hash, MtString* key);

void mt_hash_clear(MtHash* hash);
void mt_hash_free(MtHash* hash);

#endif // MT_HASH_H
