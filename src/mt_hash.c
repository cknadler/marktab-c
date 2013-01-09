#include "mt_hash.h"

#include <assert.h>
#include <stdlib.h>

#include "mt_tree.h"

// Allocates and initializes the buckets for a hash based on its current size
#define mt_hash_alloc_and_init_buckets(hash) { \
    hash->buckets = (MtHashElement *) malloc(sizeof(MtHashElement) * hash->size); \
    size_t i; \
    for (i = 0; i < hash->size; ++i) \
    { \
      hash->buckets[i].is_tree = false; \
      hash->buckets[i].data = NULL; \
    } \
  }

// An implementation of the murmur hash algorithm
// http://code.google.com/p/smhasher/wiki/MurmurHash3
// This is, for the most part, borrowed from the the HKL language
// See https://github.com/hkl/hkl
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

  switch(len)
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
// MtHashElement
//

typedef struct MtHashElement
{
  bool is_tree;
  void* data;
} MtHashElement;

//
// MtHash
//

static bool
mt_hash_move(MtPair* pair, void* new_hash)
{
  assert(pair != NULL);
  assert(new_hash != NULL);

  mt_hash_move_pair((MtHash*) new_hash, pair);

  return false;
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
  MtHashElement* old_buckets = hash->buckets;

  // Create and init new buckets
  mt_hash_alloc_and_init_buckets(hash);

  // Move the old buckets
  MtHashElement* element = NULL;

  size_t i;
  for (i = 0; i < old_size; ++i)
  {
    element = &old_buckets[i];

    if (element->is_tree)
    {
      // Beware, magic and function pointers...
      mt_tree_traverse((MtTree*) element->data, mt_hash_move, hash);
    }
    else if (element->data)
    {
      mt_hash_move((MtPair *) element->data, hash);
    }
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
  mt_hash_alloc_and_init_buckets(hash);

  return hash;
}

void
mt_hash_insert(MtHash* hash, MtString* key, void* value)
{
  assert(hash != NULL);
  assert(key != NULL);

  if (key->hash == 0)
    key->hash = Murmur3(mt_string_get_utf8(key), mt_string_get_length(key));

  size_t index = key->hash % hash->size;

  MtHashElement* element = &hash->buckets[index];
  assert(element != NULL);

  // Collision handling
  if (element->data != NULL)
  {
    if (element->is_tree)
    {
      // Collision, tree already made
      mt_tree_insert((MtTree *) element->data, key, value);
    }
    else
    {
      // First collision, make a tree
      MtPair* pair = element->data;

      element->data = mt_tree_new();
      mt_tree_move_pair((MtTree *) element->data, pair);
      
      ++hash->length;

      mt_tree_insert((MtTree *) element->data, key, value);

      element->is_tree = true;
    }
  }
  // No collision
  else
  {
    element->data = mt_pair_new(key, value);

    ++hash->length;

    if (hash->length >= 0.75 * hash->size)
      mt_hash_double_size(hash);
  }
}

void
mt_hash_move_pair(MtHash* hash, MtPair* pair)
{
  assert(hash != NULL);
  assert(pair != NULL);

  // Can only be moved from another hash
  // Had better have a key if that is the case
  assert(pair->key->hash != 0);

  size_t index = pair->key->hash % hash->size;

  MtHashElement* element = &hash->buckets[index];
  assert(element != NULL);

  // Collision
  if (element->data != NULL)
  {
    // Standard Collision
    if (element->is_tree)
    {
      mt_tree_move_pair((MtTree*) element->data, pair);
    }
    // First Collision
    else
    {
      // Have to handle both pairs being moved into the tree
      MtPair* old_pair = element->data;

      element->data = mt_tree_new();

      mt_tree_move_pair((MtTree*) element->data, pair);
      mt_tree_move_pair((MtTree*) element->data, old_pair);

      element->is_tree = true;
    }
  }
  // No Collision
  else
  {
    element->data = pair;

    ++hash->length;

    // Grow the table if is 75% or more full
    if (hash->length >= 0.75 * hash->size)
      mt_hash_double_size(hash);
  }
}

MtPair* mt_hash_search(MtHash* hash, MtString* key)
{
  assert(hash != NULL);
  assert(key != NULL);

  if (key->hash == 0)
    key->hash = Murmur3(mt_string_get_utf8(key), mt_string_get_length(key));

  size_t index = key->hash % hash->size;

  MtHashElement* element = &hash->buckets[index];
  assert(element != NULL);

  // Key Collides
  if (element->data != NULL)
  {
    // If it is a tree, search the tree for the key
    if (element->is_tree)
    {
      return mt_tree_search((MtTree*) element->data, key);
    }
    else if (mt_string_compare(((MtPair*) element->data)->key, key) == 0)
    {
      return (MtPair*) element->data;
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

  MtHashElement* element = &hash->buckets[index];
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
  }
}

void
mt_hash_traverse(MtHash* hash, bool(*fn)(MtPair*, void*), void* data)
{
  size_t i;
  MtHashElement* element = NULL;

  for (i = 0; i < hash->size; ++i)
  {
    element = &hash->buckets[i];

    if (element->is_tree)
      mt_tree_traverse((MtTree*) element->data, fn, data);
    
    else if (element->data)
      fn((MtPair*) element->data, data);
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
