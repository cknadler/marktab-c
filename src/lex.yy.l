%{

#include <stdio.h>

#include "marktab.h"

extern int yyerror(const char* msg);

%}

%%

"//".*[\n]             yylineno++;/* Ignore Comments */
[ \t\r]                /* Ignore Whitespace */
[\n]                   yylineno++;


\A[1-9]                return MT_T_STRING;
\A[0-9]{1,2}           return MT_T_FRET;

":"                    return MT_T_COLON;
"("                    return MT_T_LEFT_PAREN;
")"                    return MT_T_RIGHT_PAREN;
"end"                  return MT_T_END;

"x"                    return MT_T_MUTE;

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
