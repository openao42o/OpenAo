%{
#include <iostream.h>
#include <string.h>
#include "SymTab.h"

extern int CalculatorTesterror(const char* s);
extern int CalculatorTestlex();
%}

%union {
	double dval;
	struct symtab *symp;
}

%token <symp>	NAME
%token <dval>	NUMBER
%left       '-' '+'
%left       '/' '*'
	/*%nonassoc   UMINUS*/
%type <dval>	expression

%%
statement_list:	statement '\n'
		|	statement_list statement '\n'
		;
statement:  NAME '=' expression	{ $1->value = $3; }
        |   expression  { printf("= %g\n", $1); }
        ;
expression: expression '+' expression   { $$ = $1 + $3; }
        |   expression '-' expression   { $$ = $1 - $3; }
        |   expression '*' expression   { $$ = $1 * $3; }
        |   expression '/' expression
                {
                    if ($3 == 0.0)
                        yyerror("divided by zero");
                    else
                        $$ = $1 / $3;
                }
//      |   '-' expression %prec UMINUS { $$ = -$2; }
//		|   expression '*' expression   { $$ = $1 * $3; }
//		|   expression '*' expression   { $$ = $1 * $3; }
        |   '(' expression ')'  { $$ = $2; }
        |   NUMBER              { $$ = $1; }
		|	NAME				{ $$ = $1->value; }
        ;
%%

extern "C"
{
int CalculatorTestwrap()
{
  return 1;
}
}

int CalculatorTesterror(const char *s)
{
    cerr << "Syntax error: " << s << endl;
	return 0;
}
