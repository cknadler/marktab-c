#include <assert.h>

#include "mt_config.h"
#include "mt_alloc.h"

void mt_config_init()
{
  // Default number of strings
  mt_conf.strings = 6;

  // Default string names
  mt_conf.string_names = "eBGDAE";

  // Default line length
  mt_conf.max_line_length = 80;
}

void mt_config_set_max_line_length(int length)
{
  // TODO: Error checking here
  
  mt_conf.max_line_length = length;
}

void mt_config_set_string_names(char* names)
{
  // TODO: Error checking here

  mt_conf.string_names = names;
}

void mt_config_set_num_strings(int strings)
{
  // TODO: Error checking here
  
  mt_conf.strings = strings;
}
