#include <assert.h>

#include "mt_config.h"
#include "mt_alloc.h"

// To avoid multiple linker definitions
MtConfig* mt_global_config;

MtConfig* mt_config_new()
{
  MtConfig* config = mt_alloc_object(MtConfig);
  assert(config != NULL);

  // Default number of strings
  config->strings = 6;

  // Default string names
  config->string_names = "EADGBe";

  // Default line length
  config->line_length = 80;

  return config;
}
