#include "mt_scope.h"
#include "mt_alloc.h"
#include <assert.h>

MtScope* mt_scope_new()
{
  MtScope* scope = mt_alloc_object(MtScope);
  assert(scope != NULL);

  // Sentinel value, invalid string number
  scope->last_string = 0;

  return scope;
}

void mt_scope_free(MtScope* scope)
{
  // dealloc
}
