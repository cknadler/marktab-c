#include <assert.h>

#include "mt_conf.h"
#include "mt_alloc.h"

void mt_conf_init()
{
  // Default number of strings
  MT_CONF.strings = 6;

  // Default string names
  MT_CONF.string_names = "eBGDAE";

  // Default line length
  MT_CONF.max_line_length = 80;
}

void mt_conf_set_max_line_length(int length)
{
  // TODO: Error checking here
  
  MT_CONF.max_line_length = length;
}

void mt_conf_set_string_names(char* names)
{
  // TODO: Error checking here

  MT_CONF.string_names = names;
}

void mt_conf_set_num_strings(int strings)
{
  // TODO: Error checking here
  
  MT_CONF.strings = strings;
}
