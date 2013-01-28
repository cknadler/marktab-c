#ifndef MT_CONFIG_H
#define MT_CONFIG_H

// Convince me to change this if you can...
#define MT_CONFIG_MAX_STRINGS 27

// This is due to the max length of an unsigned char* in C
// If the output implementation changes, this could also change
#define MT_CONFIG_MAX_LINE_LENGTH 255

// If you are playing a guitar with over 99 frets...marktab is not for you
#define MT_CONFIG_MAX_FRET_DIGITS 3

typedef struct MtConfig
{
  int strings;
  char* string_names;
  int max_line_length;
} MtConfig;

MtConfig mt_conf;

void mt_config_init();

void mt_config_set_max_line_length(int length);

void mt_config_set_string_names(char* names);

void mt_config_set_num_strings(int strings);

void mt_config_set_option(char* op, void* data);

#endif
