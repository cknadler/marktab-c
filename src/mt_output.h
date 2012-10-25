#ifndef MT_OUTPUT_H
#define MT_OUTPUT_H

#include "mt_queue.h"
#include "mt_object.h"

#define MT_MAX_LINE_LENGTH 80

// MtOutputLine

typedef struct MtOutputLine
{
  char content[MT_MAX_STRINGS][MT_MAX_LINE_LENGTH];

  int length;

} MtOutputLine;

// Returns a new output line with the header prepopulated.
MtOutputLine* mt_output_line_new();

// Frees an output line and its associated resources
void mt_output_line_free(MtOutputLine* line);


// Output

// Takes in a section and outputs its contents
void mt_output_section(MtQueue* section);

void mt_output_append_line_header(MtOutputLine* line);

void mt_output_append_line_footer(MtOutputLine* line);

#endif
