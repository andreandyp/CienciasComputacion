/*
* André Michel Pozos Nieto
* Análisis y diseño de algoritmos paralelos
* Método del problema de Basilea
* Al ejecutar el algoritmo en secuencial, el tiempo de ejecución es de ~14 s con 10,000,000 iteraciones
* Al ejecutarlo en paralelo, el tiempo se reduce hasta ~3 s con 1,000,000 iteraciones y la misma precisión
* Probado en una MV con Ubuntu 18.04
* Intel Core i5 4a gen, 2 núcleos, 2 GB de RAM
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

int main(int argc, char const *argv[]){
    srand(time(NULL));
    char const valorReal[] = "3.14159";
    int const radio = 1;
    int const repeticiones = 15;

    double pCirculo = 0;
    double pi = 0;
    double x = 0;
    double y = 0;
    double valoresPi[repeticiones];

    if(argc < 2){
        printf("Añade el número de puntos a considerar, por ejemplo:\n");
        printf("%s %d\n", argv[0], 1000);
        exit(-1);
    }
    int puntos = atoi(argv[1]);

    printf("Número de puntos a contar: %d\n", puntos);
    omp_set_num_threads(2);
    
    double inicio = omp_get_wtime();

    for (int j = 0; j < repeticiones; j++) {
        #pragma omp parallel for private(x,y) reduction(+:pCirculo)
        for (int i = 0; i < puntos; i++) {

            x = (double)rand()/(double)RAND_MAX;
            y = (double)rand()/(double)RAND_MAX;
            
            x = x*radio;
            y = y*radio;
            
            if (x*x + y*y <= radio*radio) {
                pCirculo++;
            }
        }
        valoresPi[j] = 4 * pCirculo/puntos;
        pCirculo = 0;
    }
    

    double fin = omp_get_wtime();
    printf("Tiempo: %lf\n", fin - inicio);
    
    for (int j = 0; j < repeticiones; j++) {
        pi = valoresPi[j]/repeticiones + pi;
    }

    printf("%.5f (Valor calculado)\n", pi);
    printf("%s (Valor real)\n", valorReal);
    
    return 0;
}