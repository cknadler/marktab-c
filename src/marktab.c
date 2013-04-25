#include <stdio.h>
#include <stdbool.h>
#include "mtr.h"
#include "mt_output.h"

// Parser Globals

extern FILE* yyin;
extern FILE* yyout;
extern int yylex();
extern int yyparse();
extern char* yytext;
extern int yylineno;

int yywrap()
{
  return true;
}

int yyerror(const char* msg)
{
  fprintf(stderr, "On line %i: %s\n", yylineno, msg);
  return true;
}

// Public Functions

void marktab_render_tab_stdin()
{
  // TODO: implement
}

int marktab_render_tab_file(char* file)
{
  yyin = fopen(file, "r");
  if (yyin == NULL)
  {
    fprintf(stderr, "Can't open: \"%s\"\n", file);
    return 1;
  }

  // Initialize parser globals
  mtr_init();
  mt_conf_init();

  // Parse the input
  yyparse();

  // Output
  mt_output(MTR.sections);

  // TODO: Destroy parser globals

  if (yyin != NULL)
    fclose(yyin);

  return 0;
}
