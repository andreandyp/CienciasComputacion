#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>
#include "tiempo.h"
#define N 1000

int main(int argc, char const *argv[]){
	srand(time(NULL));
	int vectorA[N], vectorB[N], vectorC[N], vectorD[N];
	int res1[N], res2[N], suma[N], res3[N];
	int escalar = 0;

	//Tiempo
	double utime0, stime0, wtime0, utime1, stime1, wtime1;
	double utime2, stime2, wtime2, utime3, stime3, wtime3;
	double utime4, stime4, wtime4, utime5, stime5, wtime5;
	double utime6, stime6, wtime6, utime7, stime7, wtime7;
	double utime8, stime8, wtime8, utime9, stime9, wtime9;


	for (int i = 0; i < N; ++i){
		vectorA[i] = rand()%3;
		vectorB[i] = rand()%3;
		vectorC[i] = rand()%3;
		vectorD[i] = rand()%3;
	}

	uswtime(&utime0, &stime0, &wtime0);
	#pragma omp for
	for (int i = 0; i < N; ++i){
		res1[i] = vectorA[i] - vectorB[i];
	}
	uswtime(&utime1, &stime1, &wtime1);

	printf("VectorA - VectorB\n");
	printf("\n");
	printf("real (Tiempo total)  %.10f s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10f s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10f s\n",  stime1 - stime0);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));
	printf("\n");

	uswtime(&utime2, &stime2, &wtime2);
	#pragma omp for
	for (int i = 0; i < N; ++i){
		res2[i] = vectorB[i] + vectorC[i];
	}
	uswtime(&utime3, &stime3, &wtime3);

	printf("VectorB + vectorC\n");
	printf("\n");
	printf("real (Tiempo total)  %.10f s\n",  wtime3 - wtime2);
	printf("user (Tiempo de procesamiento en CPU) %.10f s\n",  utime3 - utime2);
	printf("sys (Tiempo en acciónes de E/S)  %.10f s\n",  stime3 - stime2);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime3 - utime2 + stime3 - stime2) / (wtime3 - wtime2));
	printf("\n");

	uswtime(&utime4, &stime4, &wtime4);
	#pragma omp for
	for (int i = 0; i < N; ++i){
		suma[i] = res1[i] + res2[i];
	}
	uswtime(&utime5, &stime5, &wtime5);

	printf("Suma\n");
	printf("\n");
	printf("real (Tiempo total)  %.10f s\n",  wtime5 - wtime4);
	printf("user (Tiempo de procesamiento en CPU) %.10f s\n",  utime5 - utime4);
	printf("sys (Tiempo en acciónes de E/S)  %.10f s\n",  stime5 - stime4);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime5 - utime4 + stime5 - stime4) / (wtime5 - wtime4));
	printf("\n");

	uswtime(&utime6, &stime6, &wtime6);
	#pragma omp for
	for (int i = 0; i < N; ++i){
		res3[i] = vectorC[i] - vectorD[i];
	}
	uswtime(&utime7, &stime7, &wtime7);

	printf("VectorC - VectorD\n");
	printf("\n");
	printf("real (Tiempo total)  %.10f s\n",  wtime7 - wtime6);
	printf("user (Tiempo de procesamiento en CPU) %.10f s\n",  utime7 - utime6);
	printf("sys (Tiempo en acciónes de E/S)  %.10f s\n",  stime7 - stime6);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime7 - utime6 + stime7 - stime6) / (wtime7 - wtime6));
	printf("\n");

	uswtime(&utime8, &stime8, &wtime8);
	#pragma omp for
	for (int i = 0; i < N; ++i){
		escalar += suma[i] * res3[i];
	}
	uswtime(&utime9, &stime9, &wtime9);

	printf("Producto\n");
	printf("\n");
	printf("real (Tiempo total)  %.10f s\n",  wtime9 - wtime8);
	printf("user (Tiempo de procesamiento en CPU) %.10f s\n",  utime9 - utime8);
	printf("sys (Tiempo en acciónes de E/S)  %.10f s\n",  stime9 - stime8);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime9 - utime8 + stime9 - stime8) / (wtime9 - wtime8));
	printf("\n");
	
	printf("Resultado: %d\n", escalar);

	return 0;
}