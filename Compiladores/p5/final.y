%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

char* concatenarStr(char* cad1, char* cad2, int tam);
char* voltearStr(char* cadena, int tam);
int cadlen(char* cad);
void calcPrefSuf(char* resta, int restaLen, int lps[]);
int kmp(char* original, char* resta);
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
%token TOK_EXP
%token TOK_LF
%token TOK_PIZQ
%token TOK_PDER
%token TOK_PC
%token TOK_ASIGN
%token TOK_MOD
%token TOK_POW
%token TOK_COMA
%token TOK_T_STR
%token TOK_T_INT
%token TOK_T_DBL
%token TOK_VAR

%left TOK_ASIGN
%left TOK_SUMA TOK_RESTA
%left TOK_MULTI TOK_DIV TOK_MOD
%left TOK_EXP
%left TOK_PIZQ TOK_PDER

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
    | TOK_T_STR TOK_VAR TOK_PC TOK_LF { printf("VARIABLES STRING\n"); }
    | TOK_T_INT TOK_VAR TOK_PC TOK_LF { printf("VARIABLES INT\n"); }
    | TOK_T_DBL TOK_VAR TOK_PC TOK_LF { printf("VARIABLES DOUBLE\n"); }
    | concatenacion TOK_LF { printf("\tResultado: %s\n", $1); }
    ;

expE: TOK_ENTERO { $$ = $1; }
    | TOK_RESTA expE { $$ = -$2; }
    | TOK_SUMA expE { $$ = $2; }
    | expE TOK_SUMA expE { $$ = $1 + $3; }
    | expE TOK_RESTA expE { $$ = $1 - $3; }
    | expE TOK_MULTI expE { $$ = $1 * $3; }
    | expE TOK_DIV expE { $$ = $1 / $3; }
    | expE TOK_MOD expE { $$ = $1 % $3; }
    | TOK_PIZQ expE TOK_PDER { $$ = $2; }
    ;

concatenacion: TOK_CADENA { $$ = $1; }
    | TOK_RESTA concatenacion {
        char* cad = voltearStr($2, cadlen($2));
        $$ = cad;
    }
    | concatenacion TOK_SUMA concatenacion {
        int nuevaLen = cadlen($1)+cadlen($3) + 1;
        char* cad = concatenarStr($1, $3, nuevaLen);
        $$ = cad;
    }
    | expE TOK_EXP concatenacion {
        int nuevaLen = cadlen($3)*$1 + 1;
        char* original = $3;
        char* cad = "";
        int i = 0;
        do{
            cad = concatenarStr(cad, original, nuevaLen);
            ++i;
        }
        while(i < $1);

        $$ = cad;
    }
    | concatenacion TOK_EXP expE {
        if($3 == 0){
            $$ = "";
            break;
        }
        int entero = ($3 < 0 ? -$3 : $3);
        int nuevaLen = cadlen($1)*entero + 1;
        char* original = $1;
        char* cad = "";
        int i = 0;
        do{
            cad = concatenarStr(cad, original, nuevaLen);
            ++i;
        }
        while(i < entero);

        if($3 < 0){
            cad = voltearStr(cad, cadlen(cad));
        }

        $$ = cad;
    }
    | concatenacion TOK_RESTA concatenacion {
        int match = kmp($1, $3);
        char* res;
        if(match != -1){
            int len = cadlen($1) - cadlen($3);
            int i = 0, j = 0;
            res = malloc(len);
            while(i < len){
                if(i != match){
                    res[i] = $1[j];
                }
                else{
                    j += cadlen($3);
                    res[i] = $1[j];
                }
                ++i;
                ++j;
            }

            //Se devuelve la resta
            $$ = res;
        }else{
            //No hay match, devuelve vacío
            $$ = "";
        }

    }
    ;

expD: TOK_DECIMAL { $$ = $1; }
    | TOK_RESTA expD { $$ = -$2; }
    | expD TOK_SUMA expD { $$ = $1 + $3; }
    | expD TOK_RESTA expD { $$ = $1 - $3; }
    | expD TOK_MULTI expD { $$ = $1 * $3; }
    | expD TOK_DIV expD { $$ = $1 / $3; }
    | TOK_POW TOK_PIZQ expD TOK_COMA expD TOK_PDER { $$ = pow($3, $5); }
    | TOK_POW TOK_PIZQ expE TOK_COMA expE TOK_PDER { $$ = pow($3, $5); }

    | expE TOK_SUMA expD { $$ = $1 + $3; }
    | expE TOK_RESTA expD { $$ = $1 - $3; }
    | expE TOK_MULTI expD { $$ = $1 * $3; }
    | expE TOK_DIV expD { $$ = $1 / $3; }
    | TOK_POW TOK_PIZQ expE TOK_COMA expD TOK_PDER { $$ = pow($3, $5); }

    | expD TOK_SUMA expE { $$ = $1 + $3; }
    | expD TOK_RESTA expE { $$ = $1 - $3; }
    | expD TOK_MULTI expE { $$ = $1 * $3; }
    | expD TOK_DIV expE { $$ = $1 / $3; }
    | TOK_PIZQ expD TOK_PDER {  $$ = $2; }
    | TOK_POW TOK_PIZQ expD TOK_COMA expE TOK_PDER { $$ = pow($3, $5); }
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
    yyparse();
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

char* concatenarStr(char* cad1, char* cad2, int tam){
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

char* voltearStr(char* cadena, int tam){
    char* volteada = malloc(tam);

    int i, j;
    for(i = tam - 1, j = 0; i >= 0; i--, j++){
        volteada[j] = cadena[i];
    }

    return volteada;
}

int kmp(char* original, char* resta) { 
    int restaLen = cadlen(resta);
    int originalLen = cadlen(original);

    int lps[restaLen];
    int j = 0;

    calcPrefSuf(resta, restaLen, lps); 

    int i = 0;
    while (i < originalLen) { 
        if (resta[j] == original[i]) { 
            j++; 
            i++; 
        } 
        if (j == restaLen) { 
            return (i - j);
        }
        else if (i < originalLen && resta[j] != original[i]) { 
            if (j != 0) 
                j = lps[j - 1]; 
            else
                i = i + 1; 
        }
    }
    return -1;
}

void calcPrefSuf(char* resta, int restaLen, int lps[]) { 
    int len = 0; 
    int i = 1; 
    lps[0] = 0;

    while (i < restaLen) { 
        if (resta[i] == resta[len]) { 
            len++; 
            lps[i] = len; 
            i++; 
        } 
        else{ 
            if (len != 0) { 
                len = lps[len - 1]; 
            } 
            else{ 
                lps[i] = len; 
                i++; 
            } 
        } 
    } 
}