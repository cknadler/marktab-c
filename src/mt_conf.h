#ifndef MT_CONF_H
#define MT_CONF_H

// Convince me to change this if you can...
#define MT_CONF_MAX_STRINGS 27

// This is due to the max length of an unsigned char* in C
// If the output implementation changes, this could also change
#define MT_CONF_MAX_LINE_LENGTH 255

// If you are playing a guitar with over 99 frets...marktab is not for you
#define MT_CONF_MAX_FRET_DIGITS 3

typedef struct MtConf
{
  int strings;
  char* string_names;
  int max_line_length;
} MtConf;

MtConf MT_CONF;

void mt_conf_init();

void mt_conf_set_max_line_length(int length);

void mt_conf_set_string_names(char* names);

void mt_conf_set_num_strings(int strings);

void mt_conf_set_option(char* op, void* data);

#endif
