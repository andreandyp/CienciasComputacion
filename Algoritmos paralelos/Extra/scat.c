#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 4

int main(int argc, char *argv[]){
	int procesos;
	int pid;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);
	int* matriz1 = malloc(N * N * sizeof(int*));
	
	for (int i = 0; i < N * N; ++i){
        matriz1[i] = rand()%4 + 1;
    }

    int* submatriz = malloc(N * (N/procesos) * sizeof(int*));

	if(pid == 0){
		for (int i = 0; i < N * N; ++i){
			printf("%d\t", matriz1[i]);
			if((i+1)%N == 0){
				printf("\n");
			}
			
	    }
		
	}

	
	MPI_Scatter(matriz1, N, MPI_INT, submatriz, N, MPI_INT, 0, MPI_COMM_WORLD);
	
	printf("Proceso %d. [%d][%d][%d][%d]\n",
		pid, submatriz[0], submatriz[1], submatriz[2], submatriz[3]);
	
	
	

	MPI_Finalize();
	return 0;
}