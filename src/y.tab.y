%{
  #include <stdio.h>

  #include "marktab.h"

  extern int yylex();
  extern int yyerror(char const*);
%}

%error-verbose

%union
{
  int       integer;
  MTString* string;
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
%token MT_T_SLIDE_DOWN            "\"
%token MT_T_HAMMER_ON             "h"
%token MT_T_PULL_OFF              "p"

%token <integer> MT_T_STRING;
%token <integer> MT_T_FRET;
%token <string>  MT_T_ID;

// Precedence

%nonassoc MODIFIER

// Grammar

%%

program:
  stmt_list

tab_list:
  stmt_list stmt
  | empty

stmt:
  note_list MT_T_END
  | chord_definition

note_list:
  note_list note optional_transition note
  | note_list note

note:
  MT_T_STRING MT_T_COLON MT_T_FRET optional_modifier
  | chord optional_modifier

optional_transition:
  MT_T_BEND
  | MT_T_SLIDE_UP
  | MT_T_SLIDE_DOWN
  | MT_T_HAMMER_ON
  | MT_T_PULL_OFF
  | empty

optional_modifer:
  MT_T_PALM_MUTE
  | MT_T_MUTE
  | MT_T_HARMONIC
  | MT_T_VIBRATO
  | empty
  
empty:
  // do nothing yo
