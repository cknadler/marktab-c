#ifndef MT_SYMBOL_H
#define MT_SYMBOL_H

#include "mt_string.h"
#include "mt_type.h"

typedef struct MtSymbol
{
  MtString* name;
  MtModifierType modifier;
} MtSymbol;

MtSymbol* mt_symbol_new(MtString* name, MtModifierType modifier);

void mt_symbol_free(MtSymbol* symbol);

#endif // MT_SYMBOL_H
