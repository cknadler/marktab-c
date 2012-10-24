#include "mt_output.h"

#include <assert.h>
#include <stdio.h>

#include "mt_alloc.h"

// MtOutputLine

MtOutputLine* mt_output_line_new()
{
  MtOutputLine* line = mt_alloc_object(MtOutputLine);
  assert(line != NULL);

  line->length = 0;

  mt_output_append_line_header(line);

  return line;
}

void mt_output_line_free(MtOutputLine* line)
{
  assert(line != NULL);

  line->length = 0;

  mt_free_object(line);

  line = NULL;
}

// Output

void mt_output_section(MtQueue* section)
{
  MtObject* object = mt_queue_dequeue(section);

  while (object != NULL)
  {
    // perform output logic
    
    object = mt_queue_dequeue(section);
  }
  
}

void mt_output_append_line_header(MtOutputLine* line)
{
  // add header content to output line 
}

void mt_output_append_line_footer(MtOutputLine* line)
{
  // add footer content to output line
}
