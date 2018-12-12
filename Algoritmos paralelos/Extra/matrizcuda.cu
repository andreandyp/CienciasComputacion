#include <stdio.h>
#include <time.h>

#define BLOCK_SIZE 4
#define div_up(x, y) ( (y) * ( ((x)+(y)-1) / (y) ) )
//Funcion multiplicacion de matric en memoria global (GM)
__global__ void Multiplica_Matrices_GM(float *C, float *A, float *B, int nfilA, int ncolA, int nfilB, int ncolB)
{
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int idy = blockIdx.y * blockDim.y + threadIdx.y;
    
    int index = idy*ncolB+idx;

    if(idy < nfilA && idx<ncolB)
    {
        float sum=0.0f;
        for(int k=0; k < ncolA; k++){
            printf("%f x %f\n", A[idy*ncolA+k], B[k*ncolB+idx]);
            sum+= A[idy*ncolA+k]*B[k*ncolB+idx];
        }
        C[index]=sum;
    }
}

int main(int argc, char const *argv[])
{
    clock_t t_ini, t_fin;
    double secs;
    srand(time(NULL));
    float *A_h, *B_h,*C_h; //punteros a matrices en el Host
    float *A_d, *B_d, *C_d; //punteros a matrices en el Device
    int nfilA = 3;
    int ncolA = 2;

    int nfilB = 2;
    int ncolB = 3;

    
    cudaEvent_t start, stop;
    A_h=(float *)malloc(nfilA * ncolA);
    B_h=(float *)malloc(nfilB * ncolB);
    C_h=(float *)malloc(nfilA * ncolB);

    for(int i=0; i<nfilA; i++)
    {
        for(int j=0; j<ncolA; j++)
        {
            A_h[i*ncolA+j] = (rand() % 4)+1;
        }
    }


    for(int i=0; i<nfilB; i++)
    {
        for(int j=0; j<ncolB; j++)
        {
            B_h[i*ncolB+j] = (rand() % 4)+1;
        }
    }

    //Aquí se pueden ver las matrices
    /*for(int i=0; i<nfilA; i++)
    {
        for(int j=0; j<ncolA; j++)
        {
            printf("%.2f\t", A_h[i*ncolA+j]);
        }
        printf("\n");
    }

    printf("--------\n");

    for(int i=0; i<nfilB; i++)
    {
        for(int j=0; j<ncolB; j++)
        {
            printf("%.2f\t", B_h[i*ncolB+j]);
        }
        printf("\n");
    }

    printf("--------\n");
    */
    
    size_t sizeA = (nfilA * ncolA) * sizeof(float);
    size_t sizeB = (nfilB * ncolB) * sizeof(float);
    size_t sizeC = (nfilA * ncolB) * sizeof(float);
    
    cudaMalloc((void**) &A_d, sizeA);
    cudaMalloc((void**) &B_d, sizeB);
    cudaMalloc((void**) &C_d, sizeC);

    
    cudaMemcpy(A_d, A_h, sizeA, cudaMemcpyHostToDevice);
    cudaMemcpy(B_d, B_h, sizeB, cudaMemcpyHostToDevice);
    
    dim3 block_size(BLOCK_SIZE, BLOCK_SIZE);
    dim3 n_blocks(div_up(ncolB,block_size.x), div_up(nfilA,block_size.y));

    t_ini=clock();
    Multiplica_Matrices_GM<<<n_blocks, block_size>>> (C_d, A_d, B_d, nfilA, ncolA, nfilB, ncolB);
    t_fin = clock();

    cudaMemcpy(C_h, C_d, sizeC, cudaMemcpyDeviceToHost);

    //Aquí se puede ver el resultado de la multiplicación
    /*printf("\n\n Matriz en cuda C: \n");
    for(int i=0; i<nfilA; i++)
    {
        for(int j=0; j<ncolB; j++)
        {
            printf("%.2f\t", C_h[i*ncolB+j]);
        }
        printf("\n");
    }*/
    

    free(A_h);
    free(B_h);
    

    cudaFree(A_d);
    cudaFree(B_d);
    cudaFree(C_d);

    secs = (double)(t_fin - t_ini)/CLOCKS_PER_SEC;
    printf("%.16g milisegundos\n", secs*1000.0);

    return 0;
}