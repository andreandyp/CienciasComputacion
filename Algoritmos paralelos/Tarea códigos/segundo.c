#include <omp.h>
#include <stdio.h>

int main (int argc, char* argv[]){
  int p, thd_id;
  #pragma omp parallel
	printf("hola mundo\n");
return 0;
}
