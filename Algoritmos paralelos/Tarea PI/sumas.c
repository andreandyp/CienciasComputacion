/*
* André Michel Pozos Nieto
* Análisis y diseño de algoritmos paralelos
* Método del problema de Basilea
* Al ejecutar el algoritmo en secuencial, el tiempo de ejecución es de ~0.08 s
* Al ejecutarlo en paralelo, el tiempo se reduce hasta ~0.05 s
* Probado en una MV con Ubuntu 18.04
* Intel Core i5 4a gen, 2 núcleos, 2 GB de RAM
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <math.h>

int main(int argc, char const *argv[]){
    char const valorReal[] = "3.141592";
    if(argc < 2){
        printf("Añade el número de elementos a sumar, por ejemplo:\n");
        printf("%s %d\n", argv[0], 1000);
        exit(-1);
    }
    int elementos = atoi(argv[1]);
    double sumaTotal = 0;

    double inicio = omp_get_wtime();
    #pragma omp parallel for reduction(+:sumaTotal)
    for (int i = 0; i < elementos; i++) {
        sumaTotal += 1/pow(i+1,2);
    }
    double fin = omp_get_wtime();
    printf("Tiempo: %lf\n", fin - inicio);
    
    double pi = sqrt(6*sumaTotal);
    printf("%.6f (Valor calculado)\n", pi);
    printf("%s (Valor real)\n", valorReal);
    
    return 0;
}