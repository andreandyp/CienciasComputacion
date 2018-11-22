%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int letrasA = 0;

%}

%%
a {
	++letrasA;
}

"\n" {
	printf("Letras A: %d\n", letrasA);
	letrasA = 0;
}

. {printf("%c", 0);}

%%
