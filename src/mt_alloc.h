#ifndef MT_ALLOC_H
#define MT_ALLOC_H

#include <stdlib.h>

#define mt_alloc_object(type) ((type *) malloc(sizeof(type)))

#define mt_free_object(obj) (free(obj))

#endif

