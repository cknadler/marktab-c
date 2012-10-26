#include <stdio.h>
#include <stdbool.h>

#include "mt.h"

extern FILE* yyin;
extern FILE* yyout;
extern int yylex();
extern int yyparse();
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

int main(int argc, const char* argv[])
{
  // If there is a filename as a parameter
  if (argv[1])
  {
    yyin = fopen(argv[1], "r");
    if (yyin == NULL)
    {
      fprintf(stderr, "Can't open: \"%s\"\n", argv[1]);
      return 1;
    }
  }

  mtr_init();

  yyparse();

  if (yyin != NULL)
    fclose(yyin);

  return 0;
}
