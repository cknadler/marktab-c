%{
  #include <stdio.h>

  #include "marktab.h"

  extern int yylex();
  extern int yyerror(char const*);
%}

%error-verbose

// Union

%union
{
  int       integer;
  MtString* string;
}

// Token List

%token MT_T_COLON                 ":"
%token MT_T_LEFT_PAREN            "("
%token MT_T_RIGHT_PAREN           ")"
%token MT_T_END                   "end"

%token MT_T_MUTE                  "x"

%token MT_T_PALM_MUTE             "m"
%token MT_T_HARMONIC              "*"
%token MT_T_VIBRATO               "~"

%token MT_T_BEND                  "b"
%token MT_T_SLIDE_UP              "/"
%token MT_T_SLIDE_DOWN            "\\"
%token MT_T_HAMMER_ON             "h"
%token MT_T_PULL_OFF              "p"

%token <integer> MT_T_STRING      "string number"
%token <integer> MT_T_FRET        "fret number"
%token <string>  MT_T_ID          "identifier"

// Precedence

  // There may be no need for precedence...
  // For now, I have no idea.

// Grammar
%%

program:
  section_list

section_list:
   section_list section
  | empty

section:
  note_or_chord_list MT_T_END
  | statement

statement:
  chord_definition

note_or_chord_list:
  note_or_chord_list note_or_chord

  | note_or_chord

  | note_or_chord transition note_or_chord

note_or_chord:
  note optional_modifier
  | chord optional_modifier
  | inline_chord optional_modifier

note:
  MT_T_STRING MT_T_COLON fret_or_mute
  {
    // $$ = mt_object_new(MT_TYPE_NOTE, $1, $3);
  }

fret_or_mute:
  MT_T_FRET
  | MT_T_MUTE

inline_chord:
  MT_T_LEFT_PAREN note_list MT_T_RIGHT_PAREN
  {

  }

note_list:
  note_list note
  | note

chord:
  MT_T_ID

chord_definition:
  MT_T_ID MT_T_COLON inline_chord

transition:
  MT_T_BEND
  | MT_T_SLIDE_UP
  | MT_T_SLIDE_DOWN
  | MT_T_HAMMER_ON
  | MT_T_PULL_OFF

optional_modifier:
  MT_T_PALM_MUTE
  | MT_T_HARMONIC
  | MT_T_VIBRATO
  | empty

empty:
  // do nothing yo
