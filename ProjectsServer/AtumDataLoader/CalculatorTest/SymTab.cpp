#include <string.h>
#include "SymTab.h"

symtab g_symtab[NSYMS];

extern int CalculatorTesterror(const char* s);

struct symtab* symlook(char *s)
{
	struct symtab *sp;
	for (sp = g_symtab; sp < &g_symtab[NSYMS]; sp++)
	{
		if (sp->name && !strcmp(sp->name, s))
			return sp;

		if (!sp->name)
		{
			sp->name = strdup(s);
			return sp;
		}
	}
	CalculatorTesterror("Too many symbols");

	return NULL;
}