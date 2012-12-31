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
  #include "mt_output.h"

  extern int yylex();
  extern int yyerror(char const*);

  // Globals
  MtQueue* current_section_queue;
  MtQueue* current_sequence_queue;
  int last_note_string;

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
  MtChord*      chord;
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

%token END 0                      "end of file"


/*
  Type declaration
*/

/*
%type <integer> transition optional_modifier optional_multiplier
%type <note>    note
%type <object>  object
*/


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
  line_list

line_list:
  line_list line
  | line

line:
  tab_line
  | definition_line
  | print_line

definition_line:
  definition MT_T_NEWLINE

print_line:
  MT_T_PRINT_LINE

tab_line:
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
  | inline_chord optional_modifier
  | inline_sequence optional_modifier
  | MT_T_ID optional_modifier
  | MT_T_REST

/* NOTES */
note:
  MT_T_NUMBER MT_T_COLON MT_T_NUMBER
  | MT_T_NUMBER MT_T_COLON MT_T_MUTE
  | MT_T_NUMBER
  | MT_T_MUTE

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
  MT_T_BEND 
  | MT_T_SLIDE_UP
  | MT_T_SLIDE_DOWN
  | MT_T_HAMMER_ON
  | MT_T_PULL_OFF

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
  MT_T_MULTIPLY MT_T_NUMBER
  | empty
 
optional_modifier:
  MT_T_PALM_MUTE 
  | MT_T_HARMONIC
  | MT_T_VIBRATO
  | empty

/* EMTPY */
empty: /* Do nothing yo */ ;
