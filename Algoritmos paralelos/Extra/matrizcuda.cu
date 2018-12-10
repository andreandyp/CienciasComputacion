#include <stdio.h>
#include <time.h>
#include <time.h>
#define BLOCK_SIZE 4
#define div_up(x, y) ( (y) * ( ((x)+(y)-1) / (y) ) )
//Funcion multiplicacion de matric en memoria global (GM)
__global__ void Multiplica_Matrices_GM(float *C, float *A, float *B, int nfil, int ncol)
{
    int idx=blockIdx.x * blockDim.x + threadIdx.x;
    int idy=blockIdx.y * blockDim.y + threadIdx.y;
    int index=idy*ncol+idx;
    if(idx<nfil && idx<ncol)
    {
        float sum=0.0f;
        for(int k=0; k<ncol; k++){
            sum+=A[idy*ncol+k]*B[k*ncol+idx];
        }
        C[index]=sum;
    }
}

int main(int argc, char const *argv[])
{
    clock_t t_ini, t_fin;
    double secs;
    t_ini=clock();
    srand(time(NULL));
    float *A_h, *B_h,*C_h; //punteros a matrices en el Host
    float *A_d, *B_d, *C_d; //punteros a matrices en el Device
    int nfil = 1000;
    int ncol = 1000;
    int N = nfil*ncol; //numero de elementos de la mtriz

    //GPU time
    cudaEvent_t start, stop;
    float time;
    size_t size = N * sizeof(float);
    A_h=(float *)malloc(size);//se pide memoria al host
    B_h=(float *)malloc(size);
    C_h=(float *)malloc(size);

    //Inicializamos las matrices a,b en el host
    for(int i=0; i<nfil; i++)
    {
        for(int j=0; j<ncol; j++)
        {
            A_h[i*ncol+j] = (rand() % 4)+1;
            B_h[i*ncol+j] = (rand() % 4)+1;
        }
    }
    
    //Pedimos memoria en el Device
    cudaMalloc((void**) &A_d, size);
    cudaMalloc((void**) &B_d, size);
    cudaMalloc((void**) &C_d, size);

    //Copiamos la matriz a y b del Host al Device
    cudaMemcpy(A_d, A_h, size, cudaMemcpyHostToDevice);
    cudaMemcpy(B_d, B_h, size, cudaMemcpyHostToDevice);
    
    //Realizamos el cálculo en el Device
    dim3 block_size(BLOCK_SIZE, BLOCK_SIZE);
    dim3 n_blocks(div_up(ncol,block_size.x), div_up(nfil,block_size.y));

    Multiplica_Matrices_GM<<<n_blocks, block_size>>> (C_d, A_d, B_d, nfil, ncol);

    //Pasamos el resultado del Device al Host
    cudaMemcpy(C_h, C_d, size, cudaMemcpyDeviceToHost);

    //imprimos el resultado.
    /*printf("\n\n Matriz en cuda C: \n");
    for(int i=0; i<10; i++)
    {
        for(int j=0; j<=10; j++)
        {
            printf("%.2f\t", C_h[i*ncol+j]);
        }
        printf("\n");
    }*/
    //Liberamos la memoria del Host
    free(A_h);
    free(B_h);
    free(C_h);

    //Liberamos la memoria del Device
    cudaFree(A_d);
    cudaFree(B_d);
    cudaFree(C_d);

    t_fin=clock();
    secs=(double)(t_fin -t_ini)/CLOCKS_PER_SEC;
    printf("%.16g milisegundos\n", secs*1000.0);

    return 0;
}