#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#define N 10000

int matriz1[N][N];
int matriz2[N][N];
int resultado[N][N];

int main(int argc, char *argv[]){
    srand(time(NULL));
    MPI_Status status;
    int pid,world_size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    printf("Llenando matrices...\n");
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j) {
            matriz1[i][j] = rand()%8 + 1;
            matriz2[i][j] = rand()%8 + 1;
        }
    }

    int i = 0;
    printf("Enviando a subprocesos...\n");
    if (pid == 0) {
        for (i = 1; i < world_size; i++){
            MPI_Send(&matriz1[i * N / world_size][0], N * N / world_size, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(matriz2, N * N, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(&matriz1[pid * N / world_size][0], N * N / world_size, MPI_INT, i, 0, MPI_COMM_WORLD, 0);
        MPI_Recv(matriz2, N * N, MPI_INT, i, 0, MPI_COMM_WORLD, 0);
    }

    printf("Multiplicando...\n");
    double inicio = MPI_Wtime();
    for(i = pid * N / world_size; i < (pid + 1) * N/world_size; ++i){
        for (int j =0 ; j < N; j++){
            resultado[i][j] = 0;
            for (int k = 0; k < N; k++){
              resultado[i][j] += matriz1[i][k] * matriz2[k][j];
            }
        }
    }
    double fin = MPI_Wtime();

    printf("Recibiendo...\n");
    if (pid != 0){
        MPI_Send(&resultado[(pid) * N/world_size][0], N*N/world_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    else{
        for (i=1; i<world_size; i++){
            MPI_Recv(&resultado[i * N/world_size][0], N * N / world_size, MPI_INT, i, 0, MPI_COMM_WORLD, 0);
        }
    }


    MPI_Barrier(MPI_COMM_WORLD);
    if (pid == 0){
        printf("Tiempo: %lf\n", fin - inicio);
    }

    MPI_Finalize();
    return 0;
}