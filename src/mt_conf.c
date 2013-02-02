#include <assert.h>

#include "mt_conf.h"
#include "mt_alloc.h"

// private protos
static void
mt_conf_set_tuning(MtString* value);

static void
mt_conf_set_line_length(MtString* value);

void
mt_conf_init()
{
  // Default number of strings
  MT_CONF.strings = 6;

  // Default string names
  MT_CONF.string_names = "eBGDAE";

  // Default line length
  MT_CONF.max_line_length = 80;
}

void
mt_conf_set_option(MtString* id, MtString* value)
{
  assert(id != NULL);
  assert(value != NULL);

  // set config options
  if (mt_string_compare_utf8(id, "tuning"))
    mt_conf_set_tuning(value);

  if (mt_string_compare_utf8(id, "line_length"))
    mt_conf_set_line_length(value);

  // Free strings
  mt_string_free(id);
  mt_string_free(value);
}

static void
mt_conf_set_tuning(MtString* value)
{
  // TODO: Set tuning based on conf value
  // This should inherently set the string number as well

  /*
  int length = mt_string_get_length(value);
  const char* raw_data = mt_string_get_utf8(value);
  char* tuning = malloc(MT_CONF_MAX_STRINGS);

  // TODO: Use a regex parser
  size_t i;
  for (i = 0; i < length; ++i)
  {
    int ascii_value = raw_data[i];
  }

  free(tuning);
  */
}

static void
mt_conf_set_line_length(MtString* value)
{
  // TODO: Set the line length based on conf value
}
