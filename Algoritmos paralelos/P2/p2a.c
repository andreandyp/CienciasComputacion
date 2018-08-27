/*
* André Michel Pozos Nieto
* Analysis and design of parallel algorithms - 3CV3
* Producto interno de 2 arreglos, código secuencial
**/

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 10

int main(int argc, char const *argv[]){
	int a[N], b[N], c[N];
	for (int i = 0; i < N; ++i){
		a[i] = i*2;
		b[i] = i*3;
	}
	#pragma omp parallel for
		for (int i = 0; i < N; ++i)
		{
			c[i] = a[i]*b[i];
		}
	for (int i = 0; i < N; ++i)
	{
		printf("%d x %d = %d\n", a[i], b[i], c[i]);
	}
	return 0;
}