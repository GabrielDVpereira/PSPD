#include <stdio.h>
#include <mpi.h>
#include <sys/types.h>
#include <strings.h>
#include <stdlib.h>

// somar 2 vetores de 16 unidades: 
// - master inicializa vetores de entrada
// - os workers realizam as somas principais
// - o master recebe as parciais e imprime o valor resultante 

// compilar: mpicc <nome_arquivo>
// executar: mpirun -n <numero_instancias> <caminho_do_binario_executável>

int MASTER_RANK = 0; 
int TAG = 0;

void execMaster(); 
void execSlave(); 

int main(int argc, char * argv[]){
    int process_rank;

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); 


    if(process_rank == MASTER_RANK){
        execMaster(); 
        return 0; 
    }

    execSlave();
    return 0;
}

int ARR_SIZE = 16; 
int INT_SIZE = 1; 
void execMaster(){
    int arr1[16] = { 1, 2, 3, 4 , 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }; 
    int arr2[16] = { 1, 2, 3, 4 , 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 }; 

    int nprocs; 
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs); 

    for(int i = 1; i < nprocs; i++){
        int destiny_process = i; 

        int curr_size = ARR_SIZE / (nprocs - 1); 

        int *newVector1 = malloc(sizeof(int) * curr_size); 
        int *newVector2 = malloc(sizeof(int) * curr_size); 

        int count = (curr_size) * (i - 1); 

        int offset = (curr_size); 
        int count_limit = (curr_size) * (i - 1) + offset; 

        while(count < count_limit){
            newVector1[count] = arr1[count]; 
            newVector2[count] = arr2[count]; 
            count++; 
        } 

        MPI_Send(&curr_size, INT_SIZE, MPI_INT, destiny_process, TAG, MPI_COMM_WORLD); 
        MPI_Send(newVector1, curr_size, MPI_INT, destiny_process, TAG, MPI_COMM_WORLD); 
        MPI_Send(newVector2, curr_size, MPI_INT, destiny_process, TAG, MPI_COMM_WORLD); 
        
        free(newVector1);
        free(newVector2);
    }    
    MPI_Finalize(); 
}

void execSlave(){
    int size; 
    MPI_Recv(&size, INT_SIZE, MPI_INT, MASTER_RANK, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // int *arr1 = malloc(sizeof(int) * size); 
    // int *arr2 = malloc(sizeof(int) * size); 
    // int *sum = malloc(sizeof(int) * size); 
    int arr1[8];
    int arr2[8];
    int sum[8];

    MPI_Recv(arr1, size, MPI_INT, MASTER_RANK, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(arr2, size, MPI_INT, MASTER_RANK, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    printf("\n"); 
    printf("ARR1\n");
    for(int i  = 0; i < size; i++){
        // sum[i] = arr1[i] + arr2[i]; 
        printf("%d ", arr1[i]); 
    }
    printf("\n"); 
    printf("ARR2\n");
    for(int i  = 0; i < size; i++){
        // sum[i] = arr1[i] + arr2[i]; 
        printf("%d ", arr2[i]); 
    }

     printf("\n\n"); 
    // printf("\n\n\n"); 

    // free(arr1);
    // free(arr2);
    // free(sum);

    MPI_Finalize(); 
}

