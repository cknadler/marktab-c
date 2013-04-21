#include "mt_hash.h"

#include <assert.h>
#include <stdlib.h>

#include "mt_tree.h"

// Allocates and initializes the buckets for a hash based on its current size
#define mt_hash_alloc_buckets(hash) { \
    hash->buckets = (MtPair*) malloc(sizeof(MtPair) * hash->size); \
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
  size_t index = pair->key->hash % hash->size;

  MtPair* element = &hash->buckets[index];
  assert(element != NULL);

  // Collision handling, linear probing
  if (element != NULL)
  {
    size_t offset = 0;

    // Search for an empty bucket
    while (element != NULL)
    {
      element = &hash->buckets[(index + offset) % hash->size];

      // Key already exists in the hash, replace the value
      if (mt_string_compare(element->key, pair->key) == 0)
      {
        element = pair;
        return;
      }

      ++offset;
    }
  }

  // The pair will be stored at the empty bucket that was found
  element = pair;
  ++hash->length;

  // Expand the hash if necessary
  if (hash->length >= 0.75 * hash->size)
    mt_hash_double_size(hash);
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
  MtPair* old_buckets = hash->buckets;

  // Create and init new buckets
  mt_hash_alloc_buckets(hash);

  // Move the old buckets
  MtPair* element;
  size_t i;
  for (i = 0; i < old_size; ++i)
  {
    element = &old_buckets[i];
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
  MtHash* hash = (MtHash *) malloc(sizeof(MtHash));
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

  MtPair* element = &hash->buckets[index];

  // Key Collides
  if (element != NULL)
  {
    size_t offset = 1;

    while (element != NULL)
    {
      if (mt_string_compare(key, element->key) == 0)
        return element->value;

      element = &hash->buckets[(index + offset) % hash->size];
      ++offset;
    }
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

  MtPair* element = &hash->buckets[index];
  assert(element != NULL);

  if (element->data != NULL)
  {

    if (element->is_tree)
    {
      MtTree* tree = element->data;

      // Save the size before attempting to remove element from tree
      size_t old_size = tree->size;

      mt_tree_remove(tree, key);

      // Decrement the size of the hash by the difference in the
      // old and new sizes of the tree. This shouldn't ever be more than 1.
      assert((old_size - tree->size) <= 1);
      hash->length -= old_size - tree->size;
    }
    else
    {
      mt_pair_free((MtPair*) element->data);
      --hash->length;
    }

    element->data = NULL;
  }
}

void
mt_hash_clear(MtHash* hash)
{
  assert(hash != NULL);

  MtHashElement* element = NULL;

  size_t i;
  for (i = 0; i < hash->size; ++i)
  {
    element = &hash->buckets[i];
    assert(element != NULL);

    if (element->is_tree)
      mt_tree_free((MtTree*) element->data);

    if (element->data)
      mt_pair_free((MtPair*) element->data);

    element->is_tree = false;
    element->data = NULL;
  }

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
