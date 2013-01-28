#include "mt_error.h"

#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Private protos
static void invalid_conf_option(char* op);
static void invalid_conf_data(char* op);

void mt_error_emit(MtError e, ...)
{
  va_list argp;
  va_start(argp, e);

  // Emit error message
  switch (e)
  {
    case INVALID_CONF_OPTION:
      invalid_conf_option(va_arg(argp, char*));
      break;

    case INVALID_CONF_DATA:
      invalid_conf_data(va_arg(argp, char*));
      break;

    default:
      assert(false);
      break;
  }

  // TODO: Free memory
  // Exit is called directly after this
  // All memory should be freed application wide
  va_end(argp);

  // Exit with proper error code
  // TODO: you may want to change this to `exit(1)`
  // Not exactly sure of the implications of exiting with arbitrary codes
  exit(e);
}

static void
invalid_conf_option(char* op)
{
  fprintf(stderr, "Invalid config option: %s\n", op);
}

// TODO:
// Right now when bad data is passed to a config option
// This just outputs the option and the fact that it is malformatted
// Eventually, it would be nice to print the malformatted data as a string
// in addition to the op.
static void
invalid_conf_data(char* op)
{
  fprintf(stderr, "Invalid config data for option: %s\n", op);
}
