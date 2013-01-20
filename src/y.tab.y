%{

  // StdLib
  #include <stdio.h>

  // External
  #include "mt_string.h"
  #include "mt_queue.h"
  #include "mt_hash.h"

  // Internal
  #include "mtr.h"
  #include "mt_object.h"
  #include "mt_modifier.h"
  #include "mt_note.h"
  #include "mt_chord.h"
  #include "mt_transition.h"
  #include "mt_sequence.h"
  #include "mt_symbol.h"
  #include "mt_config.h"

  #define YYDEBUG 0

  extern int yylex();
  extern int yyerror(char const*);

  // Parser locals
  MtQueue* current_chord_construction;
  MtQueue* current_chord_note_group;

%}

%start tab
%error-verbose

/*
  Union
*/

%union
{
  int           integer;
  MtString*     string;
  MtNote*       note;
  MtChord*      chord;
  MtObject*     object;
  MtSequence*   sequence;
}


/*
  Token List
*/

/* Please keep these in the same order as the tokens in lex.yy.l */

%token MT_T_NEWLINE               "newline"

%token MT_T_PRINT_LINE            "print line"

%token MT_T_MULTIPLY              "^"

%token MT_T_COLON                 ":"
%token MT_T_COMMA                 ","

%token MT_T_LEFT_BRACKET          "["
%token MT_T_RIGHT_BRACKET         "]"

%token MT_T_LEFT_PAREN            "("
%token MT_T_RIGHT_PAREN           ")"

%token MT_T_MUTE                  "x"

%token MT_T_REST                  "r"

%token MT_T_PALM_MUTE             "_"
%token MT_T_HARMONIC              "*"
%token MT_T_VIBRATO               "~"

%token MT_T_BEND                  "b"
%token MT_T_SLIDE_UP              "/"
%token MT_T_SLIDE_DOWN            "\\"
%token MT_T_HAMMER_ON             "h"
%token MT_T_PULL_OFF              "p"

%token MT_T_CONFIG_SEPARATOR      "config separator"

%token <string>  MT_T_ID          "identifier"
%token <integer> MT_T_NUMBER      "number"

%token MT_T_EOF "end of file"

/*
  Type declaration
*/

%type <integer>  transition optional_modifier multiplier
%type <note>     note
%type <chord>    inline_chord
%type <sequence> inline_sequence
%type <object>   object

/*
  Precedence
*/

// For now, there is no need for precedence.
// If there ever is, the rules would go here.

/*
  Initialization
*/

%initial-action 
{
  // For now there is no need for pre-parse initializations
  // If there ever is, they would go here
}

/*
  Grammar
*/

%%

tab:
  optional_config_header section_list

optional_config_header:
  MT_T_CONFIG_SEPARATOR config_lines MT_T_CONFIG_SEPARATOR
  |empty

config_lines:
  config_lines config_line
  | empty

/* Note this is currently stubbed, not functional yet */
config_line:
  MT_T_ID MT_T_NEWLINE

section_list:
  section_list section
  | section

section:
  { mtr_scope_push(); }
  tab_line_list
  {
    mtr_section_push();
    mtr_scope_pop();
  }
  section_break
  | section_break

section_break:
  print_line
  {
    // add print_line to tab_queue
  }
  /* TODO: FIX...this is extremely janky */
  | MT_T_EOF { YYACCEPT; }

tab_line_list:
  tab_line_list tab_line
  | tab_line

tab_line:
  definition_line
  | object_line

definition_line:
  definition_list MT_T_NEWLINE

definition_list:
  definition_list definition
  | definition

print_line:
  MT_T_PRINT_LINE

object_line:
  object_group_list MT_T_NEWLINE

object_group_list:
  object_group_list object_group
  | object_group

object_group:
  object { mtr_current_object_queue_enqueue($1); }

  | object
  {
    // TODO: Emit error if $1 is not a note or chord
    mtr_current_object_queue_enqueue($1); 
  } 
  transition_chain

  | object multiplier
  {
    int i;
    for(i = 0; i < $2; ++i)
      mtr_current_object_queue_enqueue($1);
  }

transition_chain:
  transition_chain transition_group
  | transition_group

transition_group:
  transition object
  {
    // TODO: Emit error if $2 is not a note or chord
    MtTransition* transition = mt_transition_new($1, $2);
    MtObject* transition_object = mt_object_new(MT_OBJ_TRANSITION, transition);
    mtr_current_object_queue_enqueue(transition_object); 
    mtr_current_object_queue_enqueue($2); 
  }

object:
  note optional_modifier
  {
    mt_note_set_modifier($1, $2);
    $$ = mt_object_new(MT_OBJ_NOTE, $1);
  }
  | inline_chord optional_modifier
  {
    mt_chord_set_modifier($1, $2);
    $$ = mt_object_new(MT_OBJ_CHORD, $1);
  }
  | inline_sequence optional_modifier
  {
    mt_sequence_set_modifier($1, $2);
    $$ = mt_object_new(MT_OBJ_SEQUENCE, $1);
  }
  | MT_T_ID optional_modifier
  {
    $$ = mt_object_new(MT_OBJ_SYMBOL, mt_symbol_new($1, $2));
  }
  | MT_T_REST
  { 
    $$ = mt_object_new_rest();
  }

/* 
  NOTES
*/
note:
  MT_T_NUMBER MT_T_COLON MT_T_NUMBER
  {
    mtr_set_last_note_string($1);
    $$ = mt_note_new($1, $3);
  }
  | MT_T_NUMBER MT_T_COLON MT_T_MUTE
  {
    mtr_set_last_note_string($1);
    $$ = mt_note_new_muted($1);
  }
  | MT_T_NUMBER { $$ = mt_note_new(mtr_last_note_string(), $1); }
  | MT_T_MUTE { $$ = mt_note_new_muted(mtr_last_note_string()); }

/* 
  CHORDS 
*/
inline_chord:
  MT_T_LEFT_PAREN 
  { current_chord_construction = mt_queue_new(); }
  chord_note_list MT_T_RIGHT_PAREN
  { $$ = mt_chord_new(current_chord_construction); }
 
chord_note_list:
  chord_note_list chord_note
  | chord_note
 
chord_note:
  { current_chord_note_group = mt_queue_new(); }
  chord_note_string_list MT_T_COLON chord_fret
  {
    mt_queue_dequeue_each_val(current_chord_note_group, {
      mt_queue_enqueue(current_chord_construction, val); 
    });

    mt_queue_free(current_chord_note_group);
  }
 
chord_note_string_list:
  chord_note_string_list MT_T_COMMA chord_note_string
  | chord_note_string

chord_note_string:
  MT_T_NUMBER 
  { mt_queue_enqueue(current_chord_note_group, mt_note_new_without_fret($1)); }

chord_fret:
  MT_T_NUMBER
  {
    mt_queue_each_val(current_chord_note_group, {
      mt_note_set_fret(val, $1);
    });
  }
  | MT_T_MUTE
  {
    mt_queue_each_val(current_chord_note_group, {
      mt_note_set_muted(val);
    });
  }

/*
  SEQUENCES 
*/
inline_sequence:
  MT_T_LEFT_BRACKET
  {
    mtr_scope_push();
  }
  object_group_list MT_T_RIGHT_BRACKET
  {
    MtSequence* sequence = mt_sequence_new(mtr_current_object_queue());
    mtr_scope_pop();
    $$ = sequence;
  }

/*
  TRANSITIONS
*/
transition:
  MT_T_BEND { $$ = MT_TRANSITION_BEND; }
  | MT_T_SLIDE_UP { $$ = MT_TRANSITION_SLIDE_UP; }
  | MT_T_SLIDE_DOWN { $$ = MT_TRANSITION_SLIDE_DOWN; }
  | MT_T_HAMMER_ON { $$ = MT_TRANSITION_HAMMER_ON; }
  | MT_T_PULL_OFF { $$ = MT_TRANSITION_PULL_OFF; }

/*
  DEFINITIONS
*/
definition:
  chord_definition 
  | sequence_definition

chord_definition:
  MT_T_ID MT_T_COLON inline_chord
  {
    MtObject* value = mt_object_new(MT_OBJ_CHORD, $3);

    // TODO: Check to see if key exists first

    mt_hash_insert(MTR.symbol_table, $1, value);
  }

sequence_definition:
  MT_T_ID MT_T_COLON inline_sequence
  {
    MtObject* value = mt_object_new(MT_OBJ_SEQUENCE, $3);

    // TODO: Check to see if key exists first

    mt_hash_insert(MTR.symbol_table, $1, value);
  }
 
/*
  OPTIONAL MODIFIERS AND MULTIPLIERS
*/
multiplier:
  MT_T_MULTIPLY MT_T_NUMBER { $$ = $2; }
 
optional_modifier:
  MT_T_PALM_MUTE { $$ = MT_MODIFIER_PALM_MUTE; }
  | MT_T_HARMONIC { $$ = MT_MODIFIER_HARMONIC; }
  | MT_T_VIBRATO { $$ = MT_MODIFIER_VIBRATO; }
  | empty { $$ = MT_MODIFIER_NONE; }

/* EMTPY */
empty: /* Do nothing yo */ ;
