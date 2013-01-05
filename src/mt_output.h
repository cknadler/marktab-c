#ifndef MT_OUTPUT_H
#define MT_OUTPUT_H

#include "mt_queue.h"
#include "mt_config.h"

typedef struct MtOutputLine
{
  int length;
  char content[MT_CONFIG_MAX_STRINGS][MT_CONFIG_MAX_LINE_LENGTH];
} MtOutputLine;

typedef struct MtOutput
{
  MtOutputLine* current_line;
  MtQueue* tab_lines;
} MtOutput;

// Define a single MtOutput
MtOutput MTO;

// Initialize the marktab output
void mt_output_init();

void mt_output_shutdown();

// Print the tab to stdout
void mt_output(MtQueue* sections);

#endif
