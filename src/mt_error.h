#ifndef MT_ERROR_H
#define MT_ERROR_H

#include <stdarg.h>

/**
 * A unified error handler for MarkTab
 */

typedef enum MtError
{
  INVALID_STRING,
  INVALID_CONF_OPTION,
  INVALID_CONF_DATA
} MtError;

void mt_error_emit(MtError e, ...);

#endif // MT_ERROR_H
