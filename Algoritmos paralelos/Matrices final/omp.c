#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#define N 100
int main(int argc, char const *argv[]){
	int i, j;
	srand(time(NULL));
	clock_t inicio, fin;
	double secs;
	
	inicio = clock();
	
	int vectorA[N][N], vectorB[N][N], vectorC[N][N];
	
    for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			vectorA[i][j] = (rand() % 4)+1;
			vectorB[i][j] = (rand() % 4)+1;
		}
	}

	#pragma omp parallel
	{
		int i, j, k, mul = 0;
		#pragma omp for 
		for(i = 0; i < N; i++){
			for(j = 0; j < N; j++){
				for(k = 0; k < N; k++){
					mul += (vectorA[i][k] * vectorB[j][k]);
				}
				vectorC[i][j] = mul;
				mul = 0;
			}
		}
	}
//imprimimos la matric resultante
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			printf("%d ", vectorC[i][j]);
		}
		printf("\n");
	}
	fin = clock();
	secs = (double) (fin - inicio)/CLOCKS_PER_SEC;
	printf("%.16g milisegundos\n", secs * 1000.0);
	return 0;
}
