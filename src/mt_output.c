#include "mt_output.h"

#include <assert.h>
#include <stdio.h>

#include "mt_alloc.h"

// Prototypes

void mt_output_append_object(MtOutputLine* line, MtObject* object);
void mt_output_append_spacer(MtOutputLine* line);
void mt_output_append_header(MtOutputLine* line);
void mt_output_append_footer(MtOutputLine* line);


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
  assert(section != NULL);

  MtOutputLine* line = mt_output_line_new();

  MtObject* object = mt_queue_dequeue(section);

  while (object != NULL)
  {
    mt_output_append_object(line, object);
    mt_output_append_spacer(line);
    object = mt_queue_dequeue(section);
  }
  
}

void mt_output_append_object(MtOutputLine* line, MtObject* object)
{
  assert(line != NULL);
  assert(object != NULL);

  int strings = 6;
  int i;

  switch (object->type)
  {
    case MT_OBJ_TYPE_NOTE:
    {
      MtNote* note = object->as.note; 
      
      switch (note->type)
      {
        case MT_NOTE_TYPE_NOTE:
        {
          for (i = 1; i <= strings; ++i)
          {
            if (note->string == i)
            {
              line->content[i][line->length - 1] = note->fret;
            }
            else
            {
              line->content[i][line->length - 1] = '-';
            }
          }
        }
        break;


        case MT_NOTE_TYPE_MUTE:
          for (i = 1; i <= strings; ++i)
          {
            if (note->string == i)
            {
              line->content[i][line->length - 1] = 'x';
            }
            else
            {
              line->content[i][line->length - 1] = '-';
            }
          }

          break;

        default:
          break;
      }
    }
    break;


    default:
      break;
  }

}

void mt_output_append_line_header(MtOutputLine* line)
{
  assert(line != NULL);

  assert(line->length == 0);

  int strings = 6;

  line->content[0][0] = 'e';
  line->content[1][0] = 'B';
  line->content[2][0] = 'G';
  line->content[3][0] = 'D';
  line->content[4][0] = 'A';
  line->content[5][0] = 'E';

  int i;
  for (i = 0; i < strings; ++i)
  {
    line->content[i][1] = '|';
    line->content[i][2] = '-';
  }

  line->length = 3;
}

void mt_output_append_line_footer(MtOutputLine* line)
{
  assert(line != NULL);

  assert(line->length < MT_MAX_LINE_LENGTH);

  int strings = 6;

  int i;
  for (i = 0; i < strings; ++i)
  {
    line->content[i][line->length - 1] = '|';
  }

  ++line->length;
}

void mt_output_append_spacer(MtOutputLine* line)
{
  assert(line != NULL);
  
  assert(line->length < MT_MAX_LINE_LENGTH - 1);

  int strings = 6;

  int i;
  for (i = 0; i < strings; ++i)
  {
    line->content[i][line->length - 1] = '-';
  }

  ++line->length;
}
