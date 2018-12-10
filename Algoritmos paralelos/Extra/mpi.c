#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>
#define N 4

int main(int argc, char *argv[]){
	srand(time(NULL));
	int procesos;
	int pid;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);

	int* matriz1 = malloc(N * N * sizeof(int));
	int* matriz2t = malloc(N * N * sizeof(int));
	int* matrizRes = NULL;
	int* matrizFinal = NULL;

	if(pid == 0){
		matrizRes = malloc(N * sizeof(int));
		matrizFinal = malloc(N * N * sizeof(int));
		int* matriz2 = malloc(N * N * sizeof(int));
		
		
		for (int i = 0; i < N * N; ++i){
	        matriz1[i] = rand()%2 + 1;
	        matriz2[i] = rand()%2 + 1;
	    }

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
			printf("%d\t", matriz2[i]);
			if((i+1)%N == 0){
				printf("\n");
			}
			
	    }

	    free(matriz2);

	}

	MPI_Barrier(MPI_COMM_WORLD);
	
    int* submatriz1 = malloc(N * sizeof(int));
    if(N <= procesos){
    	MPI_Scatter(matriz1, N, MPI_INT,
				submatriz1, N, MPI_INT,
				0, MPI_COMM_WORLD);	
    }
    else{
    	int* matrizTemp = malloc(N*sizeof(int));
    	if(pid == 0){
    		for(int i = 0; i < N; i++){
    			
    			for(int j = N; j < N*N; j += N){
    				for(int k = 0; k < N; k++){
    					printf("...\n");
    					matrizTemp[k] = matriz1[k+j];
    					printf("__\n");
    				}

    			}
				MPI_Send(
				    matrizTemp,
				    N,
				    MPI_INT,
				    i+1,
				    555,
				    MPI_COMM_WORLD);

    		}
    		for(int k = 0; k < N; k++){
    			submatriz1[k] = matriz1[k];
    		}

    	}
    	else{
    		MPI_Status* hola;
    		MPI_Recv(
			    matrizTemp,
			    N,
			    MPI_INT,
			    0,
			    555,
			    MPI_COMM_WORLD,
			    hola);
    		submatriz1 = matrizTemp;
    	}
    }
	

	MPI_Bcast(
    	matriz2t,
    	N*N,
    	MPI_INT,
    	0,
    	MPI_COMM_WORLD);

	double inicio = MPI_Wtime();
	for(int i = 0, l = 0; i < N * N; i+= N, ++l){
        int tmp = 0;
        for(int j = 0; j < N; j++){
            tmp += matriz2t[j+i] * submatriz1[j];
        }
        
        MPI_Gather(&tmp, 1, MPI_INT,
		    matrizRes, 1, MPI_INT,
		    0, MPI_COMM_WORLD);

        if(pid == 0){
        	for(int k = 0, n = 0; k < N; k++, n += N){
        		matrizFinal[l+n] = matrizRes[k];
        	}
        }
		
    }
    double fin = MPI_Wtime();

    free(matriz1);
    free(matriz2t);
    free(matrizRes);

    MPI_Barrier(MPI_COMM_WORLD);

	if(pid == 0){
		printf("-----\n");
		for (int i = 0; i < N * N; ++i){
			printf("%d\t", matrizFinal[i]);
			if((i+1)%N == 0){
				printf("\n");
			}
	    }

	    printf("Tiempo: %f\n", fin - inicio);
	}

	free(matrizFinal);

	MPI_Finalize();
	return 0;
}