/*
* André Michel Pozos Nieto
* Analysis and design of parallel algorithms - 3CV3
* Hola mundo secuencial compilado con -fopenmp
**/

#include <stdio.h>

#ifdef _OPENMP
	//Será secuencial aunque se compile con fopenmp
	#include <omp.h>
	#define TRUE 1
	#define FALSE 0
#else
	#define omp_get_thread_num() 0
	#define omp_get_num_threads() 1
	
#endif

int main(int argc, char const *argv[]){
	#pragma omp parallel
	{
		printf("Hola mundo\n");
	}
	return 0;
}