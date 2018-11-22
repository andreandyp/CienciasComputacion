%{
#include <stdio.h>
#include <stdlib.h>
%}

%union{
	int entero;
    double decimal;
    char* cadena;
}
%token <entero> TOK_ENTERO
%token <decimal> TOK_DECIMAL
%token <cadena> TOK_CADENA
%token TOK_SUMA
%token TOK_RESTA
%token TOK_MULTI
%token TOK_DIV
%token TOK_LF

%left TOK_SUMA TOK_RESTA
%left TOK_MULTI TOK_DIV

%type <entero> expE
%type <decimal> expD
%type <cadena> concatenacion


%%
input:
    | input line
    ;

line: TOK_LF
    | expE TOK_LF { printf("\tResultado: %d\n", $1); }
    | expD TOK_LF { printf("\tResultado: %f\n", $1); }
    ;

expE: TOK_ENTERO { $$ = $1; }
    | expE TOK_SUMA expE { $$ = $1 + $3; }
    | expE TOK_RESTA expE { $$ = $1 - $3; }
    | expE TOK_MULTI expE { $$ = $1 * $3; }
    | expE TOK_DIV expE { $$ = $1 / $3; }
    ;

concatenacion: TOK_CADENA { $$ = $1; }
    | concatenacion TOK_SUMA concatenacion {
        int i = 0, j = 0;
        char* cad = malloc(len($1)+len($3) - 1);
        while($1[i]){
            cad[j] = $1[i];
            j++;
            i++;
        }

        i = 0;
        while($3[i]){
            cad[j] = $3[i];
            j++;
            i++;
        }

        cad[j] = '\0';
        $$ = cad;
    }
    ;

expD: TOK_DECIMAL { $$ = $1; }
    | expD TOK_SUMA expD { $$ = $1 + $3; }
    | expD TOK_RESTA expD { $$ = $1 - $3; }
    | expD TOK_MULTI expD { $$ = $1 * $3; }
    | expD TOK_DIV expD { $$ = $1 / $3; }

    | expE TOK_SUMA expD { $$ = $1 + $3; }
    | expE TOK_RESTA expD { $$ = $1 - $3; }
    | expE TOK_MULTI expD { $$ = $1 * $3; }
    | expE TOK_DIV expD { $$ = $1 / $3; }

    | expD TOK_SUMA expE { $$ = $1 + $3; }
    | expD TOK_RESTA expE { $$ = $1 - $3; }
    | expD TOK_MULTI expE { $$ = $1 * $3; }
    | expD TOK_DIV expE { $$ = $1 / $3; }
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

int cadlen(char* cad){
    int i = 0;
    int len = 0;
    while(cad[i]){
        len++;
        i++;
    }

    return len;
}