
%{
#include <stdlib.h>
#include "CalculatorTest.cpp.h"
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
    { \
    int c = getchar(); \
    result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
    }

#include <math.h>
#include "SymTab.h"
%}
%%
([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?)  {
			CalculatorTestlval.dval = atof(CalculatorTesttext);
			return NUMBER;
		}
[ \t]   ;
[a-zA-Z][a-zA-Z0-9]*	{
			CalculatorTestlval.symp = symlook(CalculatorTesttext);
			return NAME;
		}
"$"		{ return 0; }
\n      |
.		return CalculatorTesttext[0];
%%
