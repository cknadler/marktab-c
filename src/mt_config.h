#ifndef MT_CONFIG_H
#define MT_CONFIG_H

typedef struct MtConfig
{
  int strings;
  char* string_names;
  int line_length;
} MtConfig;

extern MtConfig* mt_global_config;

MtConfig* mt_config_new();

#endif
