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

	if(pid == 0){
		int** matriz1 = malloc(N * N * sizeof(int*));
		int** matriz2 = malloc(N * N * sizeof(int*));
		for (int i = 0; i < N * N; ++i){
	        matriz1[i] = malloc(N * sizeof(int));
	        matriz2[i] = malloc(N * sizeof(int));
	    }

		for (int i = 0; i < N; ++i){
			for (int j = 0; j < N; ++j){
		        matriz1[i][j] = rand()%4 + 1;
		        MPI_Send(&matriz1[i][j], 1, MPI_INT, i, 50, MPI_COMM_WORLD);

		        matriz2[i][j] = rand()%4 + 1;
		        MPI_Send(&matriz2[i][j], 1, MPI_INT, j, 50, MPI_COMM_WORLD);
		    }
	    }

	    printf("generadas\n");

	}
	else{
		int** submatriz1 = malloc(N * N * sizeof(int*));
		int** submatriz2 = malloc(N * N * sizeof(int*));
		MPI_Status status;

		for (int i = 0; i < N * N; ++i){
	        submatriz1[i] = malloc(N * sizeof(int));
	        submatriz2[i] = malloc(N * sizeof(int));
	    }

		for (int i = 0; i < N; ++i){
			for (int j = 0; j < N; ++j){
		        MPI_Recv(&submatriz1[i][j], 1, MPI_INT, 0, 50, MPI_COMM_WORLD, &status);
		        MPI_Recv(&submatriz2[i][j], 1, MPI_INT, 0, 50, MPI_COMM_WORLD, &status);
		    }

		    printf("aquuiiiiiii\n");
	    }

	    printf("recibidas\n");

	    for (int i = 0; i < N; ++i){
			for (int j = 0; j < N; ++j){
				printf("%d\t", submatriz1[i][j]);
		    }

		    printf("|\t");

		    for (int j = 0; j < N; ++j){
				printf("%d\t", submatriz1[i][j]);
		    }
	    }

	    printf("\n");

	}

	MPI_Finalize();

	return 0;
}