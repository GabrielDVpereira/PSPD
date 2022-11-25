#include <stdio.h>
#include <mpi.h>

// Programa que o master envia o vetor para os slaves

int MASTER_RANK = 0; 
int ARR_SIZE = 5;
int TAG = 0; 
int main(int argc, char * argv[]){
    int process_rank; 
    int array[ARR_SIZE] = { 1, 2, 3, 4, 5 }; 

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); 

    // Excecução no processo master
    if(process_rank == MASTER_RANK){
        int nprocs; 
        MPI_Comm_size(MPI_COMM_WORLD, &nprocs); 
        
        // mandando msg para slaves
        for(int i = 1; i < nprocs; i++){
            int destiny_process = i; 
            MPI_Send(&array, ARR_SIZE, MPI_INT, destiny_process, TAG, MPI_COMM_WORLD); 
            printf("Master enviado %d para %d\n", value_sent, destiny_process); 
        }
        
        MPI_Finalize(); 
        return 0; 
    }


    // processo nos slaves 
    
    // TODO: receber o vetor e printar o valor

    MPI_Finalize(); 
    return 0;
}