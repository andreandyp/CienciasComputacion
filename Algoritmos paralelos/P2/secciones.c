 /*paralelización funcional
	generar 4 vectores en forma aleatoria y aplicar 
		los siguientes grafos como calculo*/
/*Practica 2 paralelizacion funcional */
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include "tiempo.h"
#include <stdlib.h>
#define N 1000
int main(int arg, char *argv[]){
int  A[N], B[N], C[N], D[N], H[N],E[N],F[N],I[N];
int i, r;
double utime0, stime0, wtime0, utime1, stime1, wtime1;
	double utime2, stime2, wtime2, utime3, stime3, wtime3;
	double utime4, stime4, wtime4, utime5, stime5, wtime5;
	double utime6, stime6, wtime6, utime7, stime7, wtime7;
	double utime8, stime8, wtime8, utime9, stime9, wtime9;
srand (time(NULL)); 
for(i=0; i<N;i++){
A[i]= rand()%3;
B[i]= rand()%3;
C[i]= rand()%3;
D[i]= rand()%3;
}
uswtime(&utime0, &stime0, &wtime0);
#pragma omp sections
	{
	#pragma omp section
	{

	for(i=0;i<N;i++) 
      	E[i]=A[i]-B[i];
	
	}	
#pragma omp section
	{

	for(i=0;i<N;i++) 
        F[i]=B[i]+C[i];
	
	}
	#pragma omp section 
	{

	for(i=0;i<N;i++) 
       H[i]=C[i]-D[i];
	
	}

}

	for(i=0;i<N;i++) 
     	 I[i]=E[i]+F[i];
#pragma omp reduction(+:r)
{
	for(i=0;i<N;i++)
      	r+=I[i]*H[i];
	}   
printf("posicion %d \n",r);
 uswtime(&utime1, &stime1, &wtime1);
	printf("\n");
	printf("real (Tiempo total)  %.10f s\n",  wtime1 - wtime0);
	printf("user (Tiempo de procesamiento en CPU) %.10f s\n",  utime1 - utime0);
	printf("sys (Tiempo en acciónes de E/S)  %.10f s\n",  stime1 - stime0);
	printf("CPU/Wall   %.10f %% \n",100.0 * (utime1 - utime0 + stime1 - stime0) / (wtime1 - wtime0));

	return 0;
	}
