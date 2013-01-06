#include "mt_output.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "mt_alloc.h"
#include "mt_object.h"

void mt_output_generate_tab_section(MtQueue* section);
void mt_output_print_tab();

void mt_output_header();
void mt_output_footer();
void mt_output_spacer();
void mt_output_object(MtObject* object);
void mt_output_note_block(MtNote* note);
void mt_output_spacer_line(int string, int length);
void mt_output_note(MtNote* note, int length);
void mt_output_chord(MtChord* chord);
void mt_output_sequence(MtSequence* sequence);
void mt_output_transition(MtTransition* transition);

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
      // TODO: check to see if output is possible
      mt_output_note_block(object->as.note);
      break;

    case MT_OBJ_CHORD:
      // TODO: check to see if output is possible
      mt_output_chord(object->as.chord);
      break;

    case MT_OBJ_TRANSITION:
      // TODO: check to see if output is possible
      mt_output_transition(object->as.transition);
      break;

    case MT_OBJ_SEQUENCE:
      mt_output_sequence(object->as.sequence);
      break;

    case MT_OBJ_REST:
      // TODO: check to see if output is possible
      mt_output_spacer();
      break;

    default:
      assert(false);
      break;
  }
}

// Output a note to the current line
void mt_output_note_block(MtNote* note)
{
  mt_output_note(note, note->size);

  int string;
  for (string = 0; string < mt_conf.strings; ++string)
  {
    if (MTO.current_line->content[string][MTO.current_line->length] == '\0')
    {
      mt_output_spacer_line(string, note->size);
    }
  }

  MTO.current_line->length += note->size;
}

// Outputs a single note on a single line followed by '-' until length is met
void mt_output_note(MtNote* note, int length)
{
  assert(note != NULL);
  assert(length > 0);

  char* note_chars = malloc(MT_CONFIG_MAX_FRET_DIGITS);
  int note_digits = sprintf(note_chars, "%d", note->fret);

  int pos = MTO.current_line->length;
  int end = MTO.current_line->length + length;
  
  int i; 
 
  // Print the note to the line
  for (i = 0; i < note_digits; ++i)
  {
    MTO.current_line->content[note->string][pos] = note_chars[i];
    ++pos;
  }

  // Print the note's modifier, if present

  // If length is longer than the note, fill in the rest with `-`
  while (pos < end) 
  {
    MTO.current_line->content[note->string][pos] = '-';
    ++pos;
  }

  free(note_chars);
}

void mt_output_spacer_line(int string, int length)
{
  assert(length > 0);

  int pos = MTO.current_line->length;
  int end = MTO.current_line->length + length;

  while (pos < end)
  {
    MTO.current_line->content[string][pos] = '-';
    ++pos;
  }
}

void mt_output_chord(MtChord* chord)
{
  mt_queue_each_val(chord->notes, {
    mt_output_note(val, chord->size);
  });

  int string;
  for (string = 0; string < mt_conf.strings; ++string)
  {
    if (MTO.current_line->content[string][MTO.current_line->length] == '\0')
    {
      mt_output_spacer_line(string, chord->size);
    }
  }

  MTO.current_line->length += chord->size;
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
  
  // Initialize all string arrays to be '\0'
  int string;
  for (string = 0; string < mt_conf.strings; ++string)
    memset(line->content[string], '\0', mt_conf.max_line_length);

  return line;
}

void mt_output_line_free(MtOutputLine* line)
{
  assert(line != NULL);
  mt_free_object(line);
}
