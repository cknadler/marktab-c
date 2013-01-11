#include "mt_output.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "mt_alloc.h"
#include "mt_object.h"

// Initialize the MtOutput
static void mt_output_init();
// Free the MtOutput
static void mt_output_shutdown();

// Create a new MtOutputLine
static MtOutputLine* mt_output_line_new();
// Free an MtOutputLine
static void mt_output_line_free();

// Print the output line buffer to stdout
static void mt_output_print();

// Output a section to the output line buffer
static void mt_output_section(MtQueue* section);

// Add a header to the current output line
static void mt_output_header();

// Add a footer to the current output line
static void mt_output_footer();

// Add a spacer to the current output line
static void mt_output_spacer();

// Add an object to the current output line
// The object can be a note, chord, sequence or transition
static void mt_output_object(MtObject* object);

// Add a single note to the current output line on one row
// The note is followed by '-' until length is met
// This is used for chord and note output
static void mt_output_note(MtNote* note, int length);

// Add `length` number of '-' to specified column of current output line
static void mt_output_spacer_line(int string, int length);

// Add a note to the current output line
static void mt_output_note_block(MtNote* note);

// Add a chord to the current output line
static void mt_output_chord(MtChord* chord);

// Add a sequence to the current output line
static void mt_output_sequence(MtSequence* sequence);

// Add a transition to the current output line
static void mt_output_transition(MtTransition* transition);


static void 
mt_output_init()
{
  MTO.current_line = NULL;
  MTO.line_buffer = mt_queue_new();
}

static void
mt_output_shutdown()
{
  MTO.current_line = NULL;
  mt_queue_free(MTO.line_buffer);
}

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

static void
mt_output_line_free(MtOutputLine* line)
{
  assert(line != NULL);
  mt_free_object(line);
}

static void
mt_output_print()
{
  while(MTO.line_buffer->size > 0)
  {
    MtOutputLine* line = mt_queue_dequeue(MTO.line_buffer);

    int i;
    for(i = 0; i < mt_conf.strings; ++i)
      printf("%s\n", line->content[i]);

    mt_output_line_free(line);
  }
}

static void
mt_output_section(MtQueue* section)
{
  MTO.current_line = mt_output_line_new();
  mt_output_header();

  while(section->size > 0)
  {
    MtObject* object = mt_queue_dequeue(section);
    mt_output_object(object);
  }

  mt_output_footer();
  mt_queue_enqueue(MTO.line_buffer, MTO.current_line);
  
  mt_queue_free(section);
}

// Adds a header to the current output line
static void 
mt_output_header()
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

// Adds a footer to the current output line
static void
mt_output_footer()
{
  int i;
  for(i = 0; i < mt_conf.strings; ++i)
  {
    MTO.current_line->content[i][MTO.current_line->length] = '|';
    MTO.current_line->content[i][MTO.current_line->length + 1] = '\0';
  }

  MTO.current_line->length += 1;
}

// Adds a spacer to current output line
static void
mt_output_spacer()
{
  int i;
  for(i = 0; i < mt_conf.strings; ++i)
    MTO.current_line->content[i][MTO.current_line->length] = '-';

  MTO.current_line->length += 1;
}

static void
mt_output_object(MtObject* object)
{
  // Depending on the type of object, call that object's output function
  switch(object->type)
  {
    case MT_OBJ_NOTE:
      // Output to current line
      if ((MTO.current_line->length + object->as.note->size + 1) <
          mt_conf.max_line_length)
      {
        mt_output_note_block(object->as.note);
        mt_output_spacer();
      }
      else // Make a new line, then output
      {
        mt_output_footer();
        mt_queue_enqueue(MTO.line_buffer, MTO.current_line);
        MTO.current_line = mt_output_line_new();
        mt_output_header();
      }
      break;

    case MT_OBJ_CHORD:
      // Output to current line
      if ((MTO.current_line->length + object->as.chord->size + 1) < 
          mt_conf.max_line_length)
      {
        mt_output_chord(object->as.chord);
        mt_output_spacer();
      }
      else // Make a new line, then output
      {
        mt_output_footer();
        mt_queue_enqueue(MTO.line_buffer, MTO.current_line);
        MTO.current_line = mt_output_line_new();
        mt_output_header();
      }
      break;

    case MT_OBJ_TRANSITION:
      if ((MTO.current_line->length + 2) < mt_conf.max_line_length)
      {
        mt_output_transition(object->as.transition);
        mt_output_spacer();
      }
      else
      {
        mt_output_footer();
        mt_queue_enqueue(MTO.line_buffer, MTO.current_line);
        MTO.current_line = mt_output_line_new();
        mt_output_header();
      }
      break;

    case MT_OBJ_SEQUENCE:
      mt_output_sequence(object->as.sequence);
      break;

    case MT_OBJ_REST:
      if ((MTO.current_line->length + 2) < mt_conf.max_line_length)
      {
        mt_output_spacer();
        mt_output_spacer();
      }
      else
      {
        mt_output_footer();
        mt_queue_enqueue(MTO.line_buffer, MTO.current_line);
        MTO.current_line = mt_output_line_new();
        mt_output_header();
      }
      break;

    default:
      assert(false);
      break;
  }
}

static void
mt_output_note(MtNote* note, int length)
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
  char modifier = '\0';

  switch (note->modifier)
  {
    case MT_MODIFIER_PALM_MUTE:
      modifier = 'm';
      break;

    case MT_MODIFIER_HARMONIC:
      modifier = '*';
      break;

    case MT_MODIFIER_VIBRATO:
      modifier = '~';
      break;

    default:
      break;
  }

  if (modifier != '\0')
  {
    MTO.current_line->content[note->string][pos] = modifier;
    ++pos;
  }

  // If length is longer than the note, fill in the rest with `-`
  while (pos < end) 
  {
    MTO.current_line->content[note->string][pos] = '-';
    ++pos;
  }

  free(note_chars);
}

static void
mt_output_spacer_line(int string, int length)
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

static void
mt_output_note_block(MtNote* note)
{
  mt_output_note(note, note->size);

  int string;
  for (string = 0; string < mt_conf.strings; ++string)
  {
    if (MTO.current_line->content[string][MTO.current_line->length] == '\0')
      mt_output_spacer_line(string, note->size);
  }

  MTO.current_line->length += note->size;
}

static void
mt_output_chord(MtChord* chord)
{
  assert(chord != NULL);

  mt_queue_each_val(chord->notes, {
    mt_output_note(val, chord->size);
  });

  int string;
  for (string = 0; string < mt_conf.strings; ++string)
  {
    if (MTO.current_line->content[string][MTO.current_line->length] == '\0')
      mt_output_spacer_line(string, chord->size);
  }

  MTO.current_line->length += chord->size;
}

static void
mt_output_sequence(MtSequence* sequence)
{
  mt_queue_each_val(sequence->objects, {
    mt_output_object(val);
  });
}

static void
mt_output_transition(MtTransition* transition)
{
  assert(transition != NULL); 

  int pos = MTO.current_line->length;

  int string;
  for (string = 0; string < mt_conf.strings; ++string)
  {
    if (transition->content[string] != '\0')
      MTO.current_line->content[string][pos] = transition->content[string];
    else
      MTO.current_line->content[string][pos] = '-';
  }

  MTO.current_line->length += 1;
}


// 
// Public
//

void
mt_output(MtQueue* sections)
{
  // Initialize the output
  mt_output_init();

  // Iterate through every section
  while (sections->size > 0)
  {
    // Generate tab for a single section
    MtQueue* section = mt_queue_dequeue(sections);
    mt_output_section(section);
  }

  mt_output_print();

  mt_output_shutdown();
}
