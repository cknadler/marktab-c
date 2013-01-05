#include "mt_output.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "mt_alloc.h"
#include "mt_object.h"

void mt_output_generate_tab_section(MtQueue* section);
void mt_output_print_tab();

void mt_output_header();
void mt_output_footer();
void mt_output_spacer();
void mt_output_object();
void mt_output_note();
void mt_output_chord();
void mt_output_sequence();
void mt_output_transition();

MtOutputLine* mt_output_line_new();
void mt_output_line_free();

//
// Public
//

void mt_output_init()
{
  MTO.tab_lines = mt_queue_new();
}

void mt_output(MtQueue* sections)
{
  mt_output_init();

  // Iterate through every section
  while (sections->size > 0)
  {
    // Generate tab for a single section
    MtQueue* section = mt_queue_dequeue(sections);
    mt_output_generate_tab_section(section);
  }

  // Output the full tab
  mt_output_print_tab();
}

//
// Private
//


/////
// MtOutput
/////

// Prints all tab lines stored in the MTO
void mt_output_print_tab()
{
  while(MTO.tab_lines->size > 0)
  {
    MtOutputLine* line = mt_queue_dequeue(MTO.tab_lines);

    int i;
    for(i = 0; i < mt_conf.strings; ++i)
    {
      printf("%s\n", line->content[i]);
    }

    mt_output_line_free(line);
  }
}

// Generates tab lines from a section and adds them to the
// output line queue of MTO
void mt_output_generate_tab_section(MtQueue* section)
{
  MTO.current_line = mt_output_line_new();

  mt_output_header();

  while(section->size > 0)
  {
    MtObject* object = mt_queue_dequeue(section);
    mt_output_object(object);
    mt_output_spacer();
  }

  // Output section footer
  mt_output_footer();
  mt_queue_enqueue(MTO.tab_lines, MTO.current_line);
  
  mt_queue_free(section);
}

// Adds a header to the current output line
void mt_output_header()
{
  int i;
  for(i = 0; i < mt_conf.strings; ++i)
  {
    MTO.current_line->content[i][0] = mt_conf.string_names[i]; 
    MTO.current_line->content[i][1] = '|';
    MTO.current_line->content[i][2] = '-';
  }

  MTO.current_line->length += 3;  
}

// Adds a spacer to current output line
void mt_output_spacer()
{
  int i;
  for(i = 0; i < mt_conf.strings; ++i)
  {
    MTO.current_line->content[i][MTO.current_line->length] = '-';
  }

  MTO.current_line->length += 1;
}

// Adds a footer to the current output line
void mt_output_footer()
{
  int i;
  for(i = 0; i < mt_conf.strings; ++i)
  {
    MTO.current_line->content[i][MTO.current_line->length] = '|';
    MTO.current_line->content[i][MTO.current_line->length + 1] = '\0';
  }

  MTO.current_line->length += 1;
}

void mt_output_object(MtObject* object)
{
  // Depending on the type of object, call that object's output function
  switch(object->type)
  {
    case MT_OBJ_NOTE:
      // Do some checks to see if output is possible
      mt_output_note(object->as.note);
      break;

    case MT_OBJ_REST:
      mt_output_spacer();
      break;

    default:
      assert(false);
      break;
  }
}

// Output a note to the current line
void mt_output_note(MtNote* note)
{
  // Convert the note fret to an array of characters
  char* note_chars = malloc(MT_CONFIG_MAX_FRET_DIGITS);
  int digits = sprintf(note_chars, "%d", note->fret);

  int i, j;
  for(i = 0; i < mt_conf.strings; ++i)
  {
    for(j = 0; j < digits; ++j)
    {
      if(i == note->string)
      {
        MTO.current_line->content[i][MTO.current_line->length + j] = 
          note_chars[j];
      }
      else
      {
        MTO.current_line->content[i][MTO.current_line->length + j] = '-';
      }
    }
  }

  MTO.current_line->length += note->size;
  free(note_chars);
}

void mt_output_chord(MtChord* chord)
{

}

void mt_output_sequence(MtSequence* sequence)
{

}

void mt_output_transition(MtTransition* transition)
{

}


/////
// MtOutputLine
/////
MtOutputLine* mt_output_line_new()
{
  MtOutputLine* line = mt_alloc_object(MtOutputLine);
  assert(line != NULL);

  line->length = 0;

  return line;
}

void mt_output_line_free(MtOutputLine* line)
{
  assert(line != NULL);
  mt_free_object(line);
}
