#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]){
	int procesos;
	int pid;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &pid);
	MPI_Comm_size(MPI_COMM_WORLD, &procesos);
	int datoGlobal = 0;
	int datoLocal;

	if(pid == 0){
		printf("Proceso maestro\n");
		printf("Llenando matrices...\n");
		for(int i = 1; i < procesos; i++){
			datoGlobal = i;
			MPI_Send(&datoGlobal, 1, MPI_INT, i, 24, MPI_COMM_WORLD);
		}
	    
	}
	else{
		MPI_Status status;
		MPI_Recv(&datoLocal, 1, MPI_INT, 0, 24, MPI_COMM_WORLD, &status);
		printf("Proceso hijo %d\nDato local %d\n", pid, datoLocal);
	}

	MPI_Finalize();
	return 0;
}