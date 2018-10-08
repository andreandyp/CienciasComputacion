#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void my_bcast(void* data, int count, MPI_Datatype datatype, int root, MPI_Comm communicator){
	int world_rank;
	MPI_Comm_rank(communicator, &world_rank);
	int world_size;
	MPI_Comm_size(communicator, &world_size);

	if(world_rank == root){
		for (int i = 0; i < world_size; ++i){
			if(i != world_rank){
				MPI_Send(data, count, datatype, i, 0, communicator);
			}
		}
	}
	else{
		MPI_Recv(data, count, datatype, root, 0, communicator, MPI_STATUS_IGNORE);
	}
}

int main(int argc, char const *argv[])
{
	int world_rank, data;

	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	if(world_rank == 0){
		data = 100;
		printf("Proceso 0 enviando datos %d\n", data);
		my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
	}
	else{
		my_bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
		printf("Proceso %d recibió los datos %d desde el proceso root\n", world_rank, data);
	}
	MPI_Finalize();

	return 0;
}