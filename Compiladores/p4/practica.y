%{
#include <stdio.h>
#include <stdlib.h>

char* cadconcat(char* cad1, char* cad2, int tam);
int cadlen(char* cad);
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
    | concatenacion TOK_LF { printf("\tResultado: %s\n", $1); }
    ;

expE: TOK_ENTERO { $$ = $1; }
    | expE TOK_SUMA expE { $$ = $1 + $3; }
    | expE TOK_RESTA expE { $$ = $1 - $3; }
    | expE TOK_MULTI expE { $$ = $1 * $3; }
    | expE TOK_DIV expE { $$ = $1 / $3; }
    ;

concatenacion: TOK_CADENA { $$ = $1; }
    | concatenacion TOK_SUMA concatenacion {
        int nuevaLen = cadlen($1)+cadlen($3) - 1;
        char* cad = cadconcat($1, $3, nuevaLen);
        $$ = cad;
    }
    | concatenacion TOK_SUMA expE {
        char numcad[21];
        sprintf(numcad, "%d", $3);
        int nuevaLen = cadlen($1)+cadlen(numcad) - 1;
        char* cad = cadconcat($1, numcad, nuevaLen);
        $$ = cad;
    }
    | expE TOK_SUMA concatenacion {
        char numcad[21];
        sprintf(numcad, "%d", $1);
        int nuevaLen = cadlen($3)+cadlen(numcad) - 1;
        char* cad = cadconcat(numcad, $3, nuevaLen);
        $$ = cad;
    }
    | expD TOK_SUMA concatenacion {
        char numcad[21];
        sprintf(numcad, "%f", $1);
        int nuevaLen = cadlen($3)+cadlen(numcad) - 1;
        char* cad = cadconcat(numcad, $3, nuevaLen);
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

char* cadconcat(char* cad1, char* cad2, int tam){
    int i = 0, j = 0;
    char* concatenada = malloc(tam);

    while(cad1[i]){
        concatenada[j] = cad1[i];
        j++;
        i++;
    }

    i = 0;
    while(cad2[i]){
        concatenada[j] = cad2[i];
        j++;
        i++;
    }

    concatenada[j] = '\0';   

    return concatenada;
}