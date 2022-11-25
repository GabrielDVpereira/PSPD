#include <stdio.h>
#include <mpi.h>

// o master passa um numero inteiro diferente para cada processo escravo

int MASTER_RANK = 0; 
int MSG_SIZE = 1;
int TAG = 0; 
int main(int argc, char * argv[]){
    int process_rank; 

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); 

    // Excecução no processo master
    if(process_rank == MASTER_RANK){
        int nprocs; 
        MPI_Comm_size(MPI_COMM_WORLD, &nprocs); 
        
        // mandando msg para slaves
        for(int i = 1; i < nprocs; i++){
            int value_sent = 10 * i; 
            int destiny_process = i; 
            MPI_Send(&value_sent, MSG_SIZE, MPI_INT, destiny_process, TAG, MPI_COMM_WORLD); 
            printf("Master enviado %d para %d\n", value_sent, destiny_process); 
        }
        
        // recebendo mensagem slaves
         for(int i = 1; i < nprocs; i++){
            int value_recieved_modified; 
            MPI_Recv(&value_recieved_modified, MSG_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD);
            printf("Master recebido %d\n", value_recieved_modified); 
        }

        MPI_Finalize(); 
        return 0; 
    }


    // processo nos slaves 
    int value_recieved; 

    MPI_Recv(&value_recieved, MSG_SIZE, MPI_INT, MASTER_RANK, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Escravo [%d] recebeu %d\n", process_rank, value_recieved);

    int modified_value = value_recieved + 2; 
    MPI_Send(&modified_value, MSG_SIZE, MPI_INT, MASTER_RANK, TAG, MPI_COMM_WORLD); 
    MPI_Finalize(); 

    return 0;
}