#ifndef MT_SYMBOL_H
#define MT_SYMBOL_H

#include "mt_string.h"
#include "mt_modifier.h"

typedef struct MtSymbol
{
  MtString* name;
  MtModifier modifier;
} MtSymbol;

MtSymbol* mt_symbol_new(MtString* name, MtModifier modifier);

void mt_symbol_free(MtSymbol* symbol);

#endif // MT_SYMBOL_H
