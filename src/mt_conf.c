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
}

static void
mt_conf_set_tuning(MtString* value)
{
  // TODO: Set tuning based on conf value
  // This should inherently set the string number as well
}

static void
mt_conf_set_line_length(MtString* value)
{
  // TODO: Set the line length based on conf value
}
