/*
* André Michel Pozos Nieto
* Analysis and design of parallel algorithms - 3CV3
* Hola mundo con número de hilos asignado desde línea de comandos (desorden)
**/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char const *argv[]){
	if(argc < 2){
		printf("Ingresa el número de hilos que quieres. Ejemplo:\n");
		printf("%s %d\n", argv[0], 5);
		exit(-1);
	}
	const int hilos = atoi(argv[1]);
	#if defined (_OPENMP)
		omp_set_num_threads(hilos);
	#endif
	#pragma omp parallel
	{
		int hilo = omp_get_thread_num();
		printf("Hola mundo desde %d de %d\n", hilo, hilos);
	}
	return 0;
}