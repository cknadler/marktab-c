%{

  // Includes

  #include <stdio.h>

  #include "mt_string.h"
  #include "mt_queue.h"

  #include "mtr.h"
  #include "mt_object.h"
  #include "mt_type.h"
  #include "mt_note.h"
  #include "mt_chord.h"
  #include "mt_transition.h"
  #include "mt_output.h"

  extern int yylex();
  extern int yyerror(char const*);

  // Globals

  MtQueue* current_section_queue;
  
%}

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
  MtQueue*      queue;
}


/*
  Token List
*/

%token MT_T_COLON                 ":"
%token MT_T_LEFT_PAREN            "("
%token MT_T_RIGHT_PAREN           ")"

%token MT_T_END                   "|"

%token MT_T_X                     "x"

%token MT_T_REST                  "r"

%token MT_T_PALM_MUTE             "m"
%token MT_T_HARMONIC              "*"
%token MT_T_VIBRATO               "~"

%token MT_T_BEND                  "b"
%token MT_T_SLIDE_UP              "/"
%token MT_T_SLIDE_DOWN            "\\"
%token MT_T_HAMMER_ON             "h"
%token MT_T_PULL_OFF              "p"

%token <integer> MT_T_NUMBER
%token <string>  MT_T_ID

%token END 0                      "end of file"


/*
  Type declaration
*/

%type <note> note
%type <object> note_or_chord
%type <chord> inline_chord
%type <queue> section
%type <integer> transition
%type <integer> optional_modifier


/*
  Precedence
*/

  // For now, there is no need for precedence.
  // If there ever is, the rules would go here.

/*
  Grammar
*/
%%

program:
  section_list

section_list:
  section_list
  {
    current_section_queue = mt_queue_new();
  }
  section
  {
    mt_output_section(current_section_queue);
    mt_queue_free(current_section_queue);
  }

  | empty

section:
  note_or_chord_list MT_T_END

  | definition
  {
    // add definition to symbol table
  }

definition:
  chord_definition

note_or_chord_list:
  note_or_chord_list note_or_chord
  {
    mt_queue_enqueue(current_section_queue, $2); 
  }

  | note_or_chord
  {
    mt_queue_enqueue(current_section_queue, $1);
  }

  | note_or_chord transition note_or_chord

note_or_chord:
  note
  {
    $$ = mt_object_new(MT_OBJ_TYPE_NOTE, $1);
  }

  | inline_chord optional_modifier
  {
    $$ = mt_object_new(MT_OBJ_TYPE_CHORD, $1);
  }

  | chord_symbol optional_modifier
  {
    // Lookup chord in hash table
    // Create an object with chord and pass it up the tree
  }

  | MT_T_REST
  {
    // Create an object with rest and pass it up the tree
  }

note:
  MT_T_NUMBER MT_T_COLON MT_T_NUMBER optional_modifier
  {
    $$ = mt_note_new($1, $3, $4);
  }

  | MT_T_NUMBER MT_T_COLON MT_T_X
  {
    $$ = mt_note_new_muted($1);
  }

inline_chord:
  MT_T_LEFT_PAREN
  {
    // Create a temporary chord queue
  }
  note_list MT_T_RIGHT_PAREN
  {
    // Free the tempoarary chord queue
  }

note_list:
  note_list note
  {
    // Add note to temporary chord queue  
  }

  | note
  {
    // Add note to temporary chord queue
  }

chord_symbol:
  MT_T_ID

chord_definition:
  MT_T_ID MT_T_COLON inline_chord

transition:
  MT_T_BEND { $$ = MT_TRANSITION_BEND; } 
  | MT_T_SLIDE_UP { $$ = MT_TRANSITION_SLIDE_UP; }
  | MT_T_SLIDE_DOWN { $$ = MT_TRANSITION_SLIDE_DOWN; }
  | MT_T_HAMMER_ON { $$ = MT_TRANSITION_HAMMER_ON; }
  | MT_T_PULL_OFF { $$ = MT_TRANSITION_PULL_OFF; }

optional_modifier:
  MT_T_PALM_MUTE { $$ = MT_MODIFIER_PALM_MUTE; }
  | MT_T_HARMONIC { $$ = MT_MODIFIER_HARMONIC; }
  | MT_T_VIBRATO { $$ = MT_MODIFIER_VIBRATO; }
  | empty { $$ = MT_MODIFIER_NONE; }

empty:
  // do nothing yo
