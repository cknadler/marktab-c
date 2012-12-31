%{

#include <stdio.h>

#include "mt.h"

extern int yyerror(const char* msg);

%}

%%

[#].*[\n]              yylineno++;/* Ignore Comments */
[ \t\r]                /* Ignore Whitespace */
^[ \t\r]*\n            yylineno++; /* Ignore empty lines */

[=].*[\n]              return MT_T_PRINT_LINE;

"^"                    return MT_T_MULTIPLY;

":"                    return MT_T_COLON;
","                    return MT_T_COMMA;

"["                    return MT_T_LEFT_BRACKET;
"]"                    return MT_T_RIGHT_BRACKET;

"("                    return MT_T_LEFT_PAREN;
")"                    return MT_T_RIGHT_PAREN;

"x"                    return MT_T_MUTE;

"r"                    return MT_T_REST;

"m"                    return MT_T_PALM_MUTE;
"*"                    return MT_T_HARMONIC;
"~"                    return MT_T_VIBRATO;

"b"                    return MT_T_BEND;
"/"                    return MT_T_SLIDE_UP;
"\\"                   return MT_T_SLIDE_DOWN;
"h"                    return MT_T_HAMMER_ON;
"p"                    return MT_T_PULL_OFF;

[a-zA-Z_][a-zA-Z_0-9]* {
  yylval.string = mt_string_new_from_utf8(yytext);
  return MT_T_ID;
}

[0-9]+ {
  sscanf(yytext, "%i", &yylval.integer);
  return MT_T_NUMBER;
}

[\n] {
  yylineno++;
  return MT_T_NEWLINE;
}

.                      yyerror(yytext);
