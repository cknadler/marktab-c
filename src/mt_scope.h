#ifndef MT_SCOPE_H
#define MT_SCOPE_H

typedef struct MtScope
{
  int last_string;
} MtScope;

MtScope* mt_scope_new();

void mt_scope_free(MtScope* scope);

#endif
