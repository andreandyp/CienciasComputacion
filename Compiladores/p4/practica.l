%{
#include <stdio.h>
#include <stdlib.h>
#include "practica.tab.h"
%}

digito [0-9]
entero {digito}+
%%

{entero} {
	printf("Número entero %s\n", yytext);
	yylval.entero = atoi(yytext);
	return TOK_ENTERO;
}

\n {
	return TOK_LF;
}

\+ {
	printf("Operador\n");
	return TOK_SUMA;
}

\- {
	printf("Operador\n");
	return TOK_RESTA;
}
\* {
	printf("Operador\n");
	return TOK_MULTI;
}
\/ {
	printf("Operador\n");
	return TOK_DIV;
}

.	;
