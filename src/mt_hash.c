#include "mt_hash.h"

#include <assert.h>
#include <stdlib.h>

// Allocates and initializes the buckets for a hash based on its current size
#define mt_hash_alloc_buckets(hash) { \
    hash->buckets = malloc(sizeof(MtPair) * hash->size); \
    size_t i; \
    for (i = 0; i < hash->size; ++i) \
      hash->buckets[i] = NULL; \
  }

// An implementation of the murmur hash algorithm
// http://code.google.com/p/smhasher/wiki/MurmurHash3
// This is, for the most part, borrowed from the the HKL language
// See https://github.com/hkl/hkl

// TODO: look into a new hashing algorithm.
static size_t Murmur3(const char* key, size_t len)
{

  #define mmix3(h,k) { \
    k *= m1; k = (k<<r1)|(k>>(sizeof(size_t)*8-r1)); \
    k *= m2; h *= 3; h ^= k; \
  }

  const size_t seed = 0xDEADBEEF;
  const size_t m1 = 0x0acffe3d, m2 = 0x0e4ef5f3, m3 = 0xa729a897;
  const size_t r1 = 11, r2 = 18, r3 = 18;

  size_t h = len + seed, k = 0;

  const size_t* dwords = (const size_t*) key;

  while(len >= sizeof(size_t))
  {
    k = *dwords++;
    mmix3(h,k);
    len -= sizeof(size_t);
  }

  const char* tail = (const char*) dwords;

  switch (len)
  {
    case 3: k ^= tail[2] << 16;
    case 2: k ^= tail[1] << 8;
    case 1:
      k ^= tail[0];
      mmix3(h,k);
  }

  h ^= h >> r2;
  h *= m3;
  h ^= h >> r3;

  return h;
}

//
// Private
//

static void mt_hash_double_size(MtHash* hash);

static void
mt_hash_insert_pair(MtHash* hash, MtPair* pair)
{
  assert(hash != NULL);
  assert(pair != NULL);

  size_t index = pair->key->hash % hash->size;
  MtPair* element = hash->buckets[index];

  // No Collision
  if (element == NULL)
  {
    hash->buckets[index] = pair;
    ++hash->length;

    // Expand the hash if necessary
    if (hash->length >= 0.75 * hash->size)
      mt_hash_double_size(hash);
  }

  // Collision handling, linear probing
  while (element != NULL)
  {
    // Key already exists in the hash, replace the value
    if (mt_string_compare(element->key, pair->key) == 0)
    {
      hash->buckets[index] = pair;
      return;
    }

    ++index;
    index %= hash->size;
    element = hash->buckets[index];
  }
}

static void
mt_hash_double_size(MtHash* hash)
{
  assert(hash != NULL);

  // Store old size
  size_t old_size = hash->size;

  // Double the size
  hash->size <<= 1;
  hash->length = 0;

  // Store old buckets
  MtPair** old_buckets = hash->buckets;

  // Create and init new buckets
  mt_hash_alloc_buckets(hash);

  // Move the old buckets
  MtPair* element;
  size_t i;
  for (i = 0; i < old_size; ++i)
  {
    element = old_buckets[i];
    if (element)
      mt_hash_insert_pair(hash, element);
  }

  // free the old buckets
  free(old_buckets);
}

//
// Public
//

MtHash*
mt_hash_new()
{
  MtHash* hash = (MtHash*) malloc(sizeof(MtHash));
  assert(hash != NULL);

  hash->size = 16;
  hash->length = 0;

  // Allocate and initialize the buckets
  mt_hash_alloc_buckets(hash);

  return hash;
}

void
mt_hash_insert(MtHash* hash, MtString* key, void* value)
{
  assert(hash != NULL);
  assert(key != NULL);

  if (key->hash == 0)
    key->hash = Murmur3(mt_string_get_utf8(key), mt_string_get_length(key));

  mt_hash_insert_pair(hash, mt_pair_new(key, value));
}

MtPair* mt_hash_search(MtHash* hash, MtString* key)
{
  assert(hash != NULL);
  assert(key != NULL);

  if (key->hash == 0)
    key->hash = Murmur3(mt_string_get_utf8(key), mt_string_get_length(key));

  size_t index = key->hash % hash->size;
  MtPair* element = hash->buckets[index];

  while (element != NULL)
  {
    if (mt_string_compare(key, element->key) == 0)
      return element;

    ++index;
    index %= hash->size;
    element = hash->buckets[index];
  }

  // Nothing was found
  return NULL;
}

void
mt_hash_remove(MtHash* hash, MtString* key)
{
  assert(hash != NULL);
  assert(key != NULL);

  if (key->hash == 0)
    key->hash = Murmur3(mt_string_get_utf8(key), mt_string_get_length(key));

  size_t index = key->hash % hash->size;
  MtPair* element = hash->buckets[index];

  while (element != NULL)
  {
    if (mt_string_compare(key, element->key) == 0)
    {
      hash->buckets[index] = NULL;
      --hash->length;
      return;
    }

    ++index;
    index %= hash->size;
    element = hash->buckets[index];
  }
}

void
mt_hash_clear(MtHash* hash)
{
  assert(hash != NULL);

  size_t i;
  for (i = 0; i < hash->size; ++i)
    hash->buckets[i] = NULL;

  hash->length = 0;
}

void
mt_hash_free(MtHash* hash)
{
  assert(hash != NULL);
  mt_hash_clear(hash);
  free(hash->buckets);
  free(hash);
}
