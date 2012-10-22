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

%token MT_T_PALM_MUTE             "m"
%token MT_T_MUTE                  "x"
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
  tab_list

tab_list:
  tab_list tab_line
  | empty

tab_line:
  tab_list MT_T_END
  | chord_definition

tab_list:
  tab_list tab optional_transition tab
  | tab_list tab
  | tab

tab:
  note optional_modifier
  | chord optional_modifier
  | inline_chord optional_modifier

note_list:
  note_list note
  | note

note:
  MT_T_STRING MT_T_COLON MT_T_FRET

chord:
  MT_T_ID

inline_chord:
  MT_T_LEFT_PAREN note_list MT_T_RIGHT_PAREN

optional_transition:
  MT_T_BEND
  | MT_T_SLIDE_UP
  | MT_T_SLIDE_DOWN
  | MT_T_HAMMER_ON
  | MT_T_PULL_OFF
  | empty

optional_modifier:
  MT_T_PALM_MUTE
  | MT_T_MUTE
  | MT_T_HARMONIC
  | MT_T_VIBRATO
  | empty

chord_definition:
  MT_T_ID MT_T_COLON inline_chord
  
empty:
  // do nothing yo
