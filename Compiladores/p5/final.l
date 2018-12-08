%{
#include <stdio.h>
#include <stdlib.h>
#include "final.tab.h"
%}

digito [0-9]
decimal {digito}*\.{digito}+
entero {digito}+
potencia pow
palabra if
t_string string
t_int int
t_double double
var [a-zA-Z][a-zA-Z0-9]+
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

{t_string} {
	printf("tipo de dato string\n");
	return TOK_T_STR;
}

{t_int} {
	printf("tipo de dato int\n");
	return TOK_T_INT;
}

{t_double} {
	printf("tipo de dato double\n");
	return TOK_T_DBL;
}
{palabra} {
	printf("Booleana\n");
	return TOK_IF;
}
{var} {
	char* cadena = malloc(cadlen(yytext));
	int i = 0;
	while(yytext[i]){
		cadena[i] = yytext[i];
		i++;
	}
	cadena[i] = '\0';
	yylval.variable = cadena;
	return TOK_VAR;
}
\< {
	printf("<\n");
	return TOK_LT;
}
\<\= {
	printf("<=\n");
	return TOK_LTE;
}
\> {
	printf(">\n");
	return TOK_GT;
}
\>\= {
	printf(">=\n");
	return TOK_GTE;
}
\=\= {
	printf("==\n");
	return TOK_CMP;
}
.	;
