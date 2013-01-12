#include "mt_symbol.h"

#include <assert.h>

#include "mt_alloc.h"

MtSymbol* mt_symbol_new(MtString* name, MtModifierType modifier)
{
  MtSymbol* symbol = mt_alloc_object(MtSymbol);
  assert(symbol != NULL);

  symbol->name = name;
  symbol->modifier = modifier;

  return symbol;
}
  
void mt_symbol_free(MtSymbol* symbol)
{
  assert(symbol != NULL);

  mt_string_free(symbol->name);
  mt_free_object(symbol);
}
