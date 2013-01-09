#include "mt_pair.h"

#include <stdlib.h>
#include <assert.h>

MtPair* mt_pair_new(MtString* key, void* value)
{
  MtPair* pair = (MtPair *) malloc(sizeof(MtPair));
  assert(pair != NULL);

  assert(key != NULL);
  assert(value != NULL);

  pair->key = key;
  pair->value = value;

  return pair;
}

void mt_pair_free(MtPair* pair)
{
  assert(pair != NULL);

  mt_string_free(pair->key);
  free(pair);
}
