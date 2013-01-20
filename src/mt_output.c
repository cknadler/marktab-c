#include "mt_output.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "mt_alloc.h"
#include "mt_object.h"
#include "mtr.h"
#include "mt_hash.h"

//
// Private protos
//

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
static void mt_output_object(MtObject* object, MtModifier override);

// Add a single note to the current output line on one row
// The note is followed by '-' until length is met
// This is used for chord and note output
static void mt_output_note_line(MtNote* note, int length, MtModifier override);

// Add `length` number of '-' to specified column of current output line
static void mt_output_spacer_line(int string, int length);

// Break the current section line
static void mt_output_line_break();

// Add a note to the current output line
static void mt_output_note(MtNote* note, MtModifier override);

// Add a chord to the current output line
static void mt_output_chord(MtChord* chord, MtModifier override);

// Add a sequence to the current output line
static void mt_output_sequence(MtSequence* sequence, MtModifier override);

// Add a transition to the current output line
static void mt_output_transition(MtTransition* transition);

// Output the contents of a symbol to the current output line
static void mt_output_symbol(MtSymbol* symbol, MtModifier override);

// 
// Public
//

void
mt_output(MtQueue* sections)
{
  // Initialize the output
  MTO.current_line = NULL;
  MTO.line_buffer = mt_queue_new();

  // Iterate through every section
  while (sections->size > 0)
  {
    // Generate tab for a single section
    MtQueue* section = mt_queue_dequeue(sections);
    mt_output_section(section);
  }

  // Print the generated output
  mt_output_print();

  // Clean up the output
  MTO.current_line = NULL;
  mt_queue_free(MTO.line_buffer);
}

//
// Private
//

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
    mt_output_object(object, MT_MODIFIER_NONE);
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
mt_output_object(MtObject* object, MtModifier override)
{
  // Depending on the type of object, call that object's output function
  switch(object->type)
  {
    case MT_OBJ_NOTE:
      mt_output_note(object->as.note, override);
      break;

    case MT_OBJ_CHORD:
      mt_output_chord(object->as.chord, override);
      break;

    case MT_OBJ_TRANSITION:
      mt_output_transition(object->as.transition);
      break;

    case MT_OBJ_SEQUENCE:
      mt_output_sequence(object->as.sequence, override);
      break;

    case MT_OBJ_REST:
      if ((MTO.current_line->length + 2) > mt_conf.max_line_length)
        mt_output_line_break();

      mt_output_spacer();
      mt_output_spacer();
      break;

    case MT_OBJ_SYMBOL:
      mt_output_symbol(object->as.symbol, override);
      break;
      
    default:
      assert(false);
      break;
  }
}

static void
mt_output_note_line(MtNote* note, int length, MtModifier override)
{
  assert(note != NULL);
  assert(length > 0);

  char* note_chars = malloc(MT_CONFIG_MAX_FRET_DIGITS);
  int note_length;
  
  // Check for regular note or muted
  if (note->type == MT_NOTE_NOTE)
  {
    note_length = sprintf(note_chars, "%d", note->fret);
  }
  else
  {
    *note_chars = 'x';
    note_length = 1;
  }

  int pos = MTO.current_line->length;
  int end = MTO.current_line->length + length;
  
  // Print the note to the line
  int i; 
  for (i = 0; i < note_length; ++i)
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
mt_output_line_break()
{
  mt_output_footer();
  mt_queue_enqueue(MTO.line_buffer, MTO.current_line);
  MTO.current_line = mt_output_line_new();
  mt_output_header();
}

static void
mt_output_note(MtNote* note, MtModifier override)
{
  assert(note != NULL);

  // Determine the modifier to apply
  MtModifier mod = note->modifier;

  if (override != MT_MODIFIER_NONE)
    mod = override;

  // Calculate chord size (including optional modifier)
  int size = note->size;

  if (mod != MT_MODIFIER_NONE)
    ++size;

  // Break line if necessary is necessary
  if ((MTO.current_line->length + size + 1) > mt_conf.max_line_length)
    mt_output_line_break();

  // Output the note line and fill in blanks
  mt_output_note_line(note, size, mod);

  int string;
  for (string = 0; string < mt_conf.strings; ++string)
  {
    if (MTO.current_line->content[string][MTO.current_line->length] == '\0')
      mt_output_spacer_line(string, size);
  }

  MTO.current_line->length += size;
  mt_output_spacer();
}

static void
mt_output_chord(MtChord* chord, MtModifier override)
{
  assert(chord != NULL);

  // Determine the modifier to apply
  MtModifier mod = chord->modifier;

  if (override != MT_MODIFIER_NONE)
    mod = override;

  // Calculate chord size (including optional modifier)
  int size = chord->size;

  if (mod != MT_MODIFIER_NONE)
    ++size;

  // Check if a line break is necessary
  if ((MTO.current_line->length + size + 1) > mt_conf.max_line_length)
    mt_output_line_break();

  // Ouput each note in the chord
  mt_queue_each_val(chord->notes, {
    mt_output_note_line(val, size, mod);
  });

  // Fill all blank lines (where notes aren't present)
  int string;
  for (string = 0; string < mt_conf.strings; ++string)
  {
    if (MTO.current_line->content[string][MTO.current_line->length] == '\0')
      mt_output_spacer_line(string, size);
  }

  MTO.current_line->length += size;
  mt_output_spacer();
}

static void
mt_output_sequence(MtSequence* sequence, MtModifier override)
{
  assert(sequence != NULL);

  // Determine modifier to apply
  MtModifier mod = sequence->modifier;

  if (override != MT_MODIFIER_NONE)
    mod = override;

  // Output each sub-object in the sequence
  mt_queue_each_val(sequence->objects, {
    mt_output_object(val, mod);
  });
} 
static void
mt_output_transition(MtTransition* transition)
{
  assert(transition != NULL); 

  // TODO: Refactor
  // Shouldn't always be max
  // Initialize transition content
  char content[MT_CONFIG_MAX_STRINGS];
  memset(content, '\0', mt_conf.strings);

  // Initalize the transition value
  char value;
  switch (transition->type)
  {
    case MT_TRANSITION_BEND:
      value = 'b';
      break;

    case MT_TRANSITION_SLIDE_UP:
      value = '/';
      break;

    case MT_TRANSITION_SLIDE_DOWN:
      value = '\\';
      break;

    case MT_TRANSITION_HAMMER_ON:
      value = 'h';
      break;

    case MT_TRANSITION_PULL_OFF:
      value = 'p';
      break;

    default:
      // Transition should be valid
      assert(false);
      break;
  }

  // Check to see if line break is necessary 
  if ((MTO.current_line->length + 2) > mt_conf.max_line_length)
    mt_output_line_break();

  // Set the content of the transtion
  switch (transition->destination->type)
  {
    case MT_OBJ_NOTE:
    {
      MtNote* note = transition->destination->as.note;
      content[note->string] = value;
    }
    break;

    case MT_OBJ_CHORD:
    {
      MtChord* chord = transition->destination->as.chord;
      mt_queue_each_val(chord->notes, {
        MtNote* note = (MtNote *) val;
        content[note->string] = value;
      });
    }
    break;

    case MT_OBJ_SYMBOL:
    {
      MtSymbol* symbol = transition->destination->as.symbol;

      MtPair* pair = mt_hash_search(MTR.symbol_table, symbol->name);
      // TODO: Error handling if symbol isn't in the table (ie: search fails)
      assert(pair != NULL);

      MtObject* object = pair->value;
      // TODO: Emit error if symbol isn't chord
      assert(object->type == MT_OBJ_CHORD);

      MtChord* chord = object->as.chord;
      mt_queue_each_val(chord->notes, {
        MtNote* note = (MtNote *) val;
        content[note->string] = value;
      });
    }
    break;

    default:
      // Transitions can only be to notes or chords
      // Symbols can contain chords, so it is possible to transition to symbol
      // TODO: Emit error
      assert(false);
      break;
  }

  // Write the transition content to the current line
  int pos = MTO.current_line->length;
      
  int string;
  for (string = 0; string < mt_conf.strings; ++string)
  {
    if (content[string] != '\0')
      MTO.current_line->content[string][pos] = content[string];
    else
      MTO.current_line->content[string][pos] = '-';
  }

  MTO.current_line->length += 1;
  mt_output_spacer();
}

static void
mt_output_symbol(MtSymbol* symbol, MtModifier override)
{
  assert(symbol != NULL);

  // Determine modifier to apply
  // TODO: Refactor
  MtModifier mod = symbol->modifier;

  if (override != MT_MODIFIER_NONE)
    mod = override;

  MtPair* pair = mt_hash_search(MTR.symbol_table, symbol->name);
  // TODO: Error handling if symbol isn't in the table (ie: search fails)
  assert(pair != NULL);

  MtObject* object = pair->value;

  switch (object->type)
  {
    case MT_OBJ_CHORD:
      mt_output_chord(object->as.chord, mod);
      break;

    case MT_OBJ_SEQUENCE:
      mt_output_sequence(object->as.sequence, mod);
      break;

    default:
      // Symbols should only be chords or sequences
      assert(false);
      break;
  }
}
