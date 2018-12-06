%{
#include <stdio.h>
#include <stdlib.h>
#include "final.tab.h"
%}

digito [0-9]
decimal {digito}*\.{digito}+
entero {digito}+
potencia pow
cadena \"[^\"]*\"
%%

{decimal} {
	printf("Número decimal %s\n", yytext);
	yylval.decimal = atof(yytext);
	return TOK_DECIMAL;
}

{entero} {
	printf("Número entero %s\n", yytext);
	yylval.entero = atoi(yytext);
	return TOK_ENTERO;
}

{cadena} {
	printf("Cadena de texto\n");
	int i = 0;
	char* cadena = malloc(cadlen(yytext));
	while(yytext[i + 2]){
		cadena[i] = yytext[i+1];
		i++;
	}
	cadena[i] = '\0';
	yylval.cadena = cadena;
	return TOK_CADENA;
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

\% {
	printf("Módulo\n");
	return TOK_MOD;
}

\^ {
	printf("Operador\n");
	return TOK_EXP;
}

\( {
	printf("Paréntesis\n");
	return TOK_PIZQ;
}

\) {
	printf("Paréntesis\n");
	return TOK_PDER;
}

\= {
	printf("Asignar\n");
	return TOK_ASIGN;
}

\; {
	printf("Punto y coma\n");
	return TOK_PC;
}

\, {
	printf("Coma\n");
	return TOK_COMA;
}

{potencia} {
	printf("Potencia\n");
	return TOK_POW;
}

.	;
