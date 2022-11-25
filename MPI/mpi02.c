#include <stdio.h>
#include <mpi.h>

// o master passa um numero inteiro diferente para cada processo escravo

int MASTER_RANK = 0; 
int MSG_SIZE = 1; 
int main(int argc, char * argv[]){
    int process_rank; 

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); 

    // Excecução no processo master
    if(process_rank == MASTER_RANK){
        int nprocs; 
        MPI_Comm_size(MPI_COMM_WORLD, &nprocs); 
        for(int i = 1; i < nprocs; i++){
            int tag =0; 
            int value_sent = 10 * i; 
            int destiny_process = i; 
            MPI_Send(&value_sent, MSG_SIZE,MPI_INT, destiny_process, tag, MPI_COMM_WORLD); 
            printf("Master enviado %d para %d\n", value_sent, destiny_process); 
        }
        MPI_Finalize(); 
        return 0; 
    }


    // processo nos slaves 
    int tag = 0; 
    int value_recieved; 
    MPI_Recv(&value_recieved, MSG_SIZE, MPI_INT, MASTER_RANK, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Escravo [%d] recebeu %d\n", process_rank, value_recieved); 
    MPI_Finalize(); 
}