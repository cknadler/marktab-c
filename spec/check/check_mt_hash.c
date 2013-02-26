#include <stdlib.h>
#include <check.h>
#include <assert.h>

#include <mt_hash.h>

int main()
{
  MtHash* hash = mt_hash_new();

  assert(hash != NULL);

  return 0;
}
