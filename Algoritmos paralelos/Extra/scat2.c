#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define N 2

int main(int argc, char *argv[]){
	int procesos;
	int pid;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);

	int* matriz1 = malloc(N * N * sizeof(int));
	int* matriz2t = malloc(N * N * sizeof(int));
	int* matrizRes = malloc(N * N * sizeof(int));

	if(pid == 0){
		int* matriz2 = malloc(N * N * sizeof(int));
		
		
		matriz1[0] = 2;
		matriz1[1] = 0;
		matriz1[2] = 1;
		matriz1[3] = 3;

		matriz2[0] = -1;
		matriz2[1] = -1;
		matriz2[2] = 5;
		matriz2[3] = 6;
		/*for (int i = 0; i < N * N; ++i){
	        matriz1[i] = rand()%2 + 1;
	        matriz2[i] = rand()%2 + 1;
	    }*/

	    int k = 0;
	    for(int i = 0; i < N; i++){
	    	for (int j = 0; j < N * N; j += N){
	    		matriz2t[k] = matriz2[i+j];
	    		++k;
	    	}
	    }

	    for (int i = 0; i < N * N; ++i){
			printf("%d\t", matriz1[i]);
			if((i+1)%N == 0){
				printf("\n");
			}
	    }

	    printf("-----\n");

	    for (int i = 0; i < N * N; ++i){
			printf("%d\t", matriz2t[i]);
			if((i+1)%N == 0){
				printf("\n");
			}
			
	    }



	    free(matriz2);

	}

	MPI_Barrier(MPI_COMM_WORLD);
	
    int* submatriz1 = malloc(N * sizeof(int));


	MPI_Scatter(matriz1, N, MPI_INT,
				submatriz1, N, MPI_INT,
				0, MPI_COMM_WORLD);

	MPI_Bcast(
    	matriz2t,
    	N*N,
    	MPI_INT,
    	0,
    	MPI_COMM_WORLD);

	for(int i = 0; i < N * N; i += N){
        int tmp = 0;
        for(int j = 0; j < N; j++){
            tmp += matriz2t[i] * submatriz1[j];
            printf("%d x %d = %d\n", submatriz1[j], matriz2t[i], matriz2t[i] * submatriz1[j]);
        }
        matrizRes[pid + i] = tmp;
    }

    free(matriz1);
    free(matriz2t);



	printf("-----\n");

    for (int i = 0; i < N * N; ++i){
		printf("%d\t", matrizRes[i]);
		if((i+1)%N == 0){
			printf("\n");
		}
    }

		/*MPI_Gather(&res, 1, MPI_INT,
		    &resGlobal, 1, MPI_INT,
		    0, MPI_COMM_WORLD);
		printf("RES %d\n", res);

		printf("Proceso %d. [%d][%d][%d][%d]\n",
			pid, submatriz1[0], submatriz1[1], submatriz1[2], submatriz1[3]);
		printf("Proceso %d. [%d][%d][%d][%d]\n",
			pid, submatriz2[0], submatriz2[1], submatriz2[2], submatriz2[3]);*/
	
	

	MPI_Finalize();
	return 0;
}