/*
* André Michel Pozos Nieto
* Análisis y diseño de algoritmos paralelos
* Multiplicación de matrices (secuencial)
* Probado en una MV con Ubuntu 18.04
* Intel Core i5 4a gen, 2 núcleos, 2 GB de RAM
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define M 10000
#define N 1000

int main(int argc, char const *argv[]){
	srand(time(NULL));
	int* resultado = (int*) malloc (sizeof(int) * N);
	int** matriz = (int**) malloc (sizeof(int*) * M);
	int* vector = (int*) malloc (sizeof(int) * N);

	for (int i = 0; i < M; i++){
		matriz[i] = (int*) malloc (N * sizeof(int));
	}

	printf("Llenando valores\n");
	for (int i = 0; i < M; ++i){

		for (int j = 0; j < N; ++j)	{
			matriz[i][j] = rand()%8 + 1;
		}
	}

	for (int i = 0; i < N; ++i){
		vector[i] = rand()%10 + 1;
	}

	printf("Multiplicando...\n");
	double inicio = omp_get_wtime();
	int tmp;
	for (int i = 0; i < M; ++i){
		tmp = 0;
		for (int j = 0; j < N; ++j){
			tmp = tmp + matriz[i][j] * vector[j];
		}
		resultado[i] = tmp;
	}
	double fin = omp_get_wtime();
    printf("Tiempo: %lf\n", fin - inicio);

    return 0;
}