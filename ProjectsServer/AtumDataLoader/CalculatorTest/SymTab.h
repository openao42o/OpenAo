#ifndef __SYM_TAB_H__
#define __SYM_TAB_H__

#define NSYMS	20

struct symtab
{
	char	*name;
	double	value;
};

extern symtab g_symtab[NSYMS];

struct symtab* symlook(char *s);

#endif