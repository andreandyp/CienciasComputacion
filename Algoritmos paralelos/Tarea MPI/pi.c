/*
mpicc pi.c -o pi
 
mpirun -np 4 pi 100

*/
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
    int iteraciones, pid, procesos;
    double PI25DT = 3.141592653589793238462643;
    double pilocal, pi, area, suma;
    
    double total, inicio, final;
    inicio = clock();
    
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &procesos);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    
    printf("Proceso %d/%d\n", pid, procesos);

    if(pid == 0){
        iteraciones = atoi(argv[1]);
        MPI_Bcast(&iteraciones, 1, MPI_INT, pid, MPI_COMM_WORLD);
    }

    if (iteraciones > 0){
        area = 1.0 / (double) iteraciones;
        suma = 0.0;
        int i = pid + 1;
        while(i <= iteraciones){
            double x = area * ((double)i - 0.5);
            suma += (4.0 / (1.0 + x*x));
            i += procesos;
        }
    }
    pilocal = area * suma;
    
    MPI_Reduce(&pilocal, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (pid == 0){
        printf("%f Valor aproximado\n", pi);
        printf("%f Valor real\n", PI25DT);
    }
    else{
        final = clock();
        total = (final-inicio)/(double) CLOCKS_PER_SEC;
        printf("total de tiempo %f \n\n",total);    
    }
    
    MPI_Finalize();
    return 0;
}