#include <stdio.h>
#include <time.h>
#include <cuda.h>

void Multiplica_Matrices_GM(float *C, float *A, float *B, int nfil, int ncol);


int main(int argc, char const *argv[])
{
    clock_t t_ini, t_fin;
    double secs;
    t_ini=clock();
    float *A_h, *B_h,*C_h; 
    float *A_d, *B_d, *C_d; 
    int nfil=12; 
    int ncol=12; 
    int N=nfil*ncol; 

    
    float time;
    size_t size =N* sizeof(float);
    A_h=(float *)malloc(size);
    B_h=(float *)malloc(size);
    C_h=(float *)malloc(size);

    
    int i, j;
    for(i=0; i<nfil; i++)
    {
        for(j=0; j<ncol; j++)
        {
            A_h[i*ncol+j]=1.0f;
            B_h[i*ncol+j]=2.0f;
                    }
    }
    
    
    cudaMalloc((void**) &A_d, size);
    cudaMalloc((void**) &B_d, size);
    cudaMalloc((void**) &C_d, size);

    
    cudaMemcpy(A_d, A_h, size, cudaMemcpyHostToDevice);
    cudaMemcpy(B_d, B_h, size, cudaMemcpyHostToDevice);
    
    
    dim3 block_size(BLOCK_SIZE,BLOCK_SIZE);
    dim3 n_blocks(div_up(ncol,block_size.x),div_up(nfil,block_size.y));

    Multiplica_Matrices_GM<<<n_blocks, block_size>>> (C_d, A_d, B_d, nfil, ncol);

    
    cudaMemcpy(C_h, C_d, size, cudaMemcpyDeviceToHost);

    
    printf("\n\n Matriz en cuda C: \n");
    for(i=0; i<10; i++)
    {
        for(j=0; j<=10; j++)
        {
            printf("%.2f", C_h[i*ncol+j]);
        }
        printf("\n");
    }
    
    free(A_h);
    free(B_h);
    free(C_h);

    
    cudaFree(A_d);
    cudaFree(B_d);
    cudaFree(C_d);

    t_fin=clock();
    secs=(double)(t_fin -t_ini)/CLOCKS_PER_SEC;
    printf("%.16g milisegundos\n", secs*1000.0);

    return 0;
}


__global__ void Multiplica_Matrices_GM(float *C, float *A, float *B, int nfil, int ncol)
{
    int idx = blockId.x * blockDim.x + threadIdx.x;
    int idy = blockId.y * blockDim.y + threadIdx.y;
    int index=idy*ncol+idx;
    int k;
    if(idy<nfil && idx<ncol)
    {
        float sum=0.0f;
        for(k=0; k<ncol; k++){
            sum+=A[idy*ncol+k]*B[k*ncol+idx];
        }
        C[index]=sum;
    }
}

