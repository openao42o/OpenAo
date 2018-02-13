#include "CalculatorTestParser.h"
#include <stdio.h>
extern FILE *CalculatorTestin;



CalculatorTestParser::
CalculatorTestParser()
{
}

CalculatorTestParser::
CalculatorTestParser(char* fname)
{
	CalculatorTestin = fopen(fname, "r");
}

int
CalculatorTestParser::
Parse()
{
	return CalculatorTestparse();
}
