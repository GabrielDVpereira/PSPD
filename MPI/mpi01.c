#include <stdio.h>
#include <mpi.h>
#include <sys/types.h>
#include <strings.h>


// Pegar no master, o rank e mensagem enviado no slave 

// compilar: mpicc <nome_arquivo>
// executar: mpirun -n <numero_instancias> <caminho_do_binario_executável>

int MASTER_RANK = 0; 

int main(int argc, char * argv[]){
    int process_rank;
    char msg[100]; 

    MPI_Init(&argc, &argv); 
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank); 

    // Execução dentro do processo slave
    if(process_rank != MASTER_RANK){
        sprintf(msg, "I'm alive\n");

        int message_size = strlen(msg) + 1; 
        int reciever_rank = MASTER_RANK;
        int tag = 0;  

        MPI_Send(msg, message_size, MPI_CHAR, reciever_rank, tag, MPI_COMM_WORLD); // mandando info para o master
        MPI_Finalize(); 
        return 0; 
    }

    // Execução do processo master
    MPI_Status status; 
    int nprocess;  
    MPI_Comm_size(MPI_COMM_WORLD, &nprocess); 
    for(int i = 1; i < nprocess; i++){
        int message_max_size = 100; 
        int tag = 0; 
        MPI_Recv(msg, message_max_size, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  // recebendo info do slave
        printf("Master recebendo mensagem de slave --> %s, processo: %d\n" , msg, status.MPI_SOURCE);
    }

    MPI_Finalize(); 
    return 0;
}