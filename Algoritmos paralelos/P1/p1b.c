/*
* André Michel Pozos Nieto
* Analysis and design of parallel algorithms - 3CV3
* Hola mundo paralelo con número de hilos predeterminado
**/

#include <stdio.h>
#include <omp.h>

int main(int argc, char const *argv[]){
	#pragma omp parallel
	{
		printf("Hola mundo\n");
	}
	return 0;
}