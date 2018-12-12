#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]){
	srand(time(NULL));
	int procesos;
	int pid;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);

	int filA = 2;
	int colA = 3;
	int filB = 3;
	int colB = 2;

	int* matriz1 = malloc(filA * colA * sizeof(int));
	int* matriz2t = malloc(filB * colB * sizeof(int));
	int* matrizRes = NULL;
	int* matrizFinal = NULL;

	if(pid == 0){
		matrizRes = malloc(procesos * sizeof(int));
		matrizFinal = malloc(filA * colB * sizeof(int));
		int* matriz2 = malloc(filA * colB * sizeof(int));
		
		
		for (int i = 0; i < filA * colA; ++i){
	        matriz1[i] = rand()%8 + 1;
	    }

	    for (int i = 0; i < filB * colB; ++i){
	        matriz2[i] = rand()%8 + 1;
	    }

	    int k = 0;
	    for(int i = 0; i < colB; i++){
	    	for (int j = 0; j < filB * colB; j += colB){
	    		matriz2t[k] = matriz2[i+j];
	    		++k;
	    	}
	    }

	    for (int i = 0; i < filA * colA; ++i){
			printf("%d\t", matriz1[i]);
			if((i+1)%colA == 0){
				printf("\n");
			}
	    }

	    printf("-----\n");

	    for (int i = 0; i < filB * colB; ++i){
			printf("%d\t", matriz2[i]);
			if((i+1)%colB == 0){
				printf("\n");
			}
			
	    }
	    printf("-----\n");

	    free(matriz2);

	}

	MPI_Barrier(MPI_COMM_WORLD);
	
    int* submatriz1 = NULL;
    if(procesos >= filA){
    	submatriz1 = malloc(procesos * sizeof(int));
    	MPI_Scatter(matriz1, colA, MPI_INT,
				submatriz1, colA, MPI_INT,
				0, MPI_COMM_WORLD);	
    }
	

	MPI_Bcast(
    	matriz2t,
    	filB * colB,
    	MPI_INT,
    	0,
    	MPI_COMM_WORLD);

	double inicio = MPI_Wtime();

	for(int i = 0, l = 0; i < filA * colB; i+= colA, ++l){
        int tmp = 0;
        for(int j = 0; j < colA; j++){
        	printf("[%d]%d x %d\n", pid, submatriz1[j], matriz2t[j+i]);
            tmp += matriz2t[j+i] * submatriz1[j];
        }

        MPI_Barrier(MPI_COMM_WORLD);
        
        MPI_Gather(&tmp, 1, MPI_INT,
		    matrizRes, 1, MPI_INT,
		    0, MPI_COMM_WORLD);

        if(pid == 0){
        	printf("[%d][%d][%d][%d]\n", matrizRes[0], matrizRes[1], matrizRes[2], matrizRes[3]);
        	for(int k = 0, n = 0; k < filA; k++, n += filA){
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
		for (int i = 0; i < filA * colB; ++i){
			printf("%d\t", matrizFinal[i]);
			if((i+1)%colB == 0){
				printf("\n");
			}
	    }

	    printf("Tiempo: %f\n", fin - inicio);
	}

	free(matrizFinal);

	MPI_Finalize();
	return 0;
}