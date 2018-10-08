/*
* André Michel Pozos Nieto
* Análisis y diseño de algoritmos paralelos
* Método de la integral
* Al ejecutar el algoritmo en secuencial, el tiempo de ejecución es de ~0.00002 s con 10,000 iteraciones
* Al ejecutarlo en paralelo, el tiempo se reduce hasta ~0.00002 s con 10,000 iteraciones y la misma precisión
* Probado en una MV con Ubuntu 18.04
* Intel Core i5 4a gen, 2 núcleos, 2 GB de RAM
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char const *argv[]){
	double area, pi, x;
	int i;

	if(argc < 2){
        printf("Añade el número de puntos a considerar, por ejemplo:\n");
        printf("%s %d\n", argv[0], 1000);
        exit(-1);
    }
    int n = atoi(argv[1]);

	area = 0.0;
	double inicio = omp_get_wtime();
	//#pragma omp parallel for private(x) reduction(+:area)
		for (i = 0; i < n; i++) {
			x = (i + 0.5)/n;
			area += 4.0/(1.0 + x*x);
		}

	double fin = omp_get_wtime();
    printf("Tiempo: %lf\n", fin - inicio);
	pi = area / n;
	printf("%.8f\n", pi);
}