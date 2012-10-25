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

// Appends the translated value of an MtObject to an MtOutputLine
void mt_output_line_append_object(MtOutputLine* line, MtObject* object);

// Appends a row of spacers to an MtOutputLine
void mt_output_line_append_spacer(MtOutputLine* line);

// Appends a header to an existing MtOutputLine
void mt_output_line_append_header(MtOutputLine* line);

// Appends a footer to an exisiting MtOutputLine
void mt_output_line_append_footer(MtOutputLine* line);

// Frees an output line and its associated resources
void mt_output_line_free(MtOutputLine* line);


// System Output

// Takes in a section and outputs its contents to a file of the same name as the input file with a .mt extension
void mt_output_section(MtQueue* section);

#endif
