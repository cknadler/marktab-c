#include <stdio.h>
#include <stdbool.h>

#include "mt.h"

#include "mt_stack.h"
#include "mt_queue.h"

extern FILE* yyin;
extern FILE* yyout;
extern int yylex();
extern int yyparse();
extern char* yytext;
extern int yylineno;

// Parser globals

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

  // Initailze the marktab runtime
  mtr_init();
  mt_config_init();

  // Initialize parser globals

  // Parse the input
  yyparse();

  // Output
  mt_output(MTR.sections);

  // Dealloc parser globals

  if (yyin != NULL)
    fclose(yyin);

  return 0;
}
