#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>
#define N 800
int main(int argc, char const *argv[]){
	int i, j;
	srand(time(NULL));
	clock_t inicio, fin;
	double secs;
	//FILE* nums = fopen("10m.txt", "r");
	
	
	
	int vectorA[N][N], vectorB[N][N], vectorC[N][N];
	
	int num1 = 0;
	int num2 = 1;
    for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			vectorA[i][j] = (rand() % 4)+1;
			vectorB[i][j] = (rand() % 4)+1;
			//fscanf(nums, "%d", &vectorA[i][j]);
			//fscanf(nums, "%d", &vectorB[i][j]);
		}
	}

	inicio = clock();
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
	fin = clock();
	secs = (double) (fin - inicio)/CLOCKS_PER_SEC;
	printf("%.16g milisegundos\n", secs * 1000.0);
	//fclose(nums);
	return 0;
}
