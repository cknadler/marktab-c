%{

  /*
    Includes
  */

  // StdLib
  #include <stdio.h>

  // External
  #include "mt_string.h"
  #include "mt_queue.h"

  // Internal
  #include "mtr.h"
  #include "mt_object.h"
  #include "mt_type.h"
  #include "mt_note.h"
  #include "mt_chord.h"
  #include "mt_transition.h"
  #include "mt_sequence.h"
  #include "mt_config.h"

  extern int yylex();
  extern int yyerror(char const*);

  // Globals
  MtQueue* tab_queue;
  MtQueue* current_tab_section_queue;
  int last_note_string;

  void initialize_parser_globals()
  {
    mt_global_config = mt_config_new();
    tab_queue = mt_queue_new();
    current_tab_section_queue = NULL;
    last_note_string = 0;
  }

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
  MtObject*     object;
  MtNote*       note;
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

%token MT_T_PALM_MUTE             "m"
%token MT_T_HARMONIC              "*"
%token MT_T_VIBRATO               "~"

%token MT_T_BEND                  "b"
%token MT_T_SLIDE_UP              "/"
%token MT_T_SLIDE_DOWN            "\\"
%token MT_T_HAMMER_ON             "h"
%token MT_T_PULL_OFF              "p"

%token <string>  MT_T_ID          "identifier"
%token <integer> MT_T_NUMBER      "number"

%token MT_T_EOF                   "end of file"


/*
  Type declaration
*/

%type <integer> transition optional_modifier optional_multiplier
%type <note>    note
%type <object>  object

/*
  Precedence
*/

// For now, there is no need for precedence.
// If there ever is, the rules would go here.


/*
  Grammar
*/

%%

tab:
  section_list

section_list:
  section_list section
  | section

section:
  {
    // New queue: current_section_queue
    // New scope
  }
  tab_line_list 
  {
    // add current_section_queue to tab_queue
    // dealloc queue
    // dealloc scope
  }
  section_break
  | section_break

section_break:
  print_line
  {
    // add print_line to tab_queue
  }
  | MT_T_EOF

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
  object_block transition object_block
  | object_block

object_block:
  object optional_multiplier

object:
  note optional_modifier
  {
    mt_note_set_modifier($1, $2);
    $$ = mt_object_new(MT_OBJ_NOTE, $1);
  }
  | inline_chord optional_modifier
  {
    // Stubbed as rest to get rid of type clashes
    $$ = mt_object_new_rest();
  }
  | inline_sequence optional_modifier
  {
    // Stubbed as rest to get rid of type clashes
    $$ = mt_object_new_rest();
  }
  | MT_T_ID optional_modifier
  {
    // Stubbed as rest to get rid of type clashes
    $$ = mt_object_new_rest();
  }
  | MT_T_REST { $$ = mt_object_new_rest(); }

/* NOTES */
note:
  MT_T_NUMBER MT_T_COLON MT_T_NUMBER
  {
    last_note_string = $1;
    $$ = mt_note_new($1, $3);
  }
  | MT_T_NUMBER MT_T_COLON MT_T_MUTE
  {
    last_note_string = $1;
    $$ = mt_note_new_muted($1);
  }
  | MT_T_NUMBER { $$ = mt_note_new(last_note_string, $1); }
  | MT_T_MUTE { $$ = mt_note_new_muted(last_note_string); }

/* CHORDS */
inline_chord:
  MT_T_LEFT_PAREN chord_note_list MT_T_RIGHT_PAREN
 
chord_note_list:
  chord_note_list chord_note
  | chord_note
 
chord_note:
  string_list MT_T_COLON MT_T_NUMBER
 
string_list:
  string_list MT_T_COMMA MT_T_NUMBER
  | MT_T_NUMBER

/* SEQUENCES */
inline_sequence:
  MT_T_LEFT_BRACKET object_group_list MT_T_RIGHT_BRACKET

/* TRANSITIONS */
transition:
  MT_T_BEND { $$ = MT_TRANSITION_BEND; }
  | MT_T_SLIDE_UP { $$ = MT_TRANSITION_SLIDE_UP; }
  | MT_T_SLIDE_DOWN { $$ = MT_TRANSITION_SLIDE_DOWN; }
  | MT_T_HAMMER_ON { $$ = MT_TRANSITION_HAMMER_ON; }
  | MT_T_PULL_OFF { $$ = MT_TRANSITION_PULL_OFF; }

/* DEFINITIONS */
definition:
  chord_definition 
  | sequence_definition

chord_definition:
  MT_T_ID MT_T_COLON inline_chord

sequence_definition:
  MT_T_ID MT_T_COLON inline_sequence
 
/* OPTIONAL MODIFIERS AND MULTIPLIERS */
optional_multiplier:
  MT_T_MULTIPLY MT_T_NUMBER { $$ = $2; }
  | empty { $$ = 1; }
 
optional_modifier:
  MT_T_PALM_MUTE { $$ = MT_MODIFIER_PALM_MUTE; }
  | MT_T_HARMONIC { $$ = MT_MODIFIER_HARMONIC; }
  | MT_T_VIBRATO { $$ = MT_MODIFIER_VIBRATO; }
  | empty { $$ = MT_MODIFIER_NONE; }

/* EMTPY */
empty: /* Do nothing yo */ ;
