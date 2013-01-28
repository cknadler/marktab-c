#ifndef MT_OUTPUT_H
#define MT_OUTPUT_H

#include "mt_queue.h"
#include "mt_conf.h"

// A line of tab output
typedef struct MtOutputLine
{
  int length;
  char content[MT_CONF_MAX_STRINGS][MT_CONF_MAX_LINE_LENGTH];
} MtOutputLine;

// A container for output lines
typedef struct MtOutput
{
  MtOutputLine* current_line;
  MtQueue* line_buffer;
} MtOutput;

// Define a single MtOutput
MtOutput MTO;

// Print the tab to stdout
void mt_output(MtQueue* sections);

#endif
