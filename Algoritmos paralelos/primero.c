#include <omp.h>
#include <stdio.h>

int main (int argc, char* argv[]){
    #pragma omp parallel
        printf("hola mundo\n");
    return 0;
}
