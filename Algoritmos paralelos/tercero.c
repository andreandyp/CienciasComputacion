#include <omp.h>
#include <stdio.h>

int main (int argc, char* argv[]){
	int iam = 0, np = 1, i = 0;
	#pragma omp parallel private(iam, np, i)
	{
		#if defined (_OPENMP)
			np = omp_get_num_threads();
			iam = omp_get_thread_num();
		#endif
		printf("Hola del hilo %d de %d\n", iam, np);
		#pragma omp for
			for (i = 0; i < (np*2); ++i){
				printf("Hilo %d, contador: %d\n", iam, i);
			}
	}
	//return 0;
}