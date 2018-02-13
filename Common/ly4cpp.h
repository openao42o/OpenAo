#ifndef __LY4CPP_H__
#define __LY4CPP_H__

extern "C" {
	int yyparse(void);
	int yylex(void);  
	int yywrap();
	int yyerror(const char *str);
};

#endif
