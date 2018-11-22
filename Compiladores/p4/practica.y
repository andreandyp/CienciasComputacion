%{
#include <stdio.h>
#include <stdlib.h>
%}

%union{
	int entero;
}
%token <entero> TOK_ENTERO
%token TOK_SUMA
%token TOK_RESTA
%token TOK_MULTI
%token TOK_DIV
%token TOK_LF

%left TOK_SUMA TOK_RESTA
%left TOK_MULTI TOK_DIV

%type <entero> exp


%%
input:
    | input line
    ;

line: TOK_LF
    | exp TOK_LF { printf("\tResultado: %d\n", $1); }
    ;

exp: TOK_ENTERO { $$ = $1; }
    | exp TOK_SUMA exp { $$ = $1 + $3; }
    | exp TOK_RESTA exp { $$ = $1 - $3; }
    | exp TOK_MULTI exp { $$ = $1 * $3; }
    | exp TOK_DIV exp { $$ = $1 / $3; }
    ;

%%
int main(int argc, char **argv){
	yyparse();
	return 0;
}

int yywrap() {
	return 1;
}

void yyerror(const char *s){
	printf("Error: %s\n", s);
}