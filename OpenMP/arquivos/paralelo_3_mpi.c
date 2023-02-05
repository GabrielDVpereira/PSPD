#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <strings.h>

#define MAX 1000000

int MASTER_RANK = 0; 


void handle_master(); 
void handle_slave(int); 

int main (int argc, char * argv[]){
	MPI_Init(&argc, &argv); 
	
	int current_process_rank; 
	MPI_Comm_rank(MPI_COMM_WORLD, &current_process_rank); 
	
	if(MASTER_RANK == current_process_rank) handle_master(); 
	else handle_slave(current_process_rank); 
	
	MPI_Finalize();

	return 0; 
}

void handle_master() {
	int number; 
	printf("Number to find: ");
	scanf("%d", &number);   

	int total_process_running; 
	MPI_Comm_size(MPI_COMM_WORLD, &total_process_running);
	
	int chunk = MAX / (total_process_running - 1); // just the slaves 

	printf("Inside master, total process: %d chunk; %d \n", total_process_running, chunk);

	int info[2] = { number, chunk };
 
	// sending message to slaves
	for(int i = 1; i < total_process_running; i++){
		MPI_Send(info, 2, MPI_INT, i, 0, MPI_COMM_WORLD);  
	}

	int *response_arr = malloc(sizeof(int) * total_process_running);
	memset(response_arr, 0, sizeof(int) * total_process_running);
	
	int sum = 0; 
	for(int i = 1; i < total_process_running; i++){
		int count; 
		MPI_Recv(&count, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		sum+= count; 
	}  
	printf("Total found: %d\n", sum); 
}

void handle_slave(int process_rank){
	int info[2]; 
	MPI_Recv(info, 2, MPI_INT, MASTER_RANK, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
	
	int chunk = info[1]; 
	int number = info[0];
	int line_size = 9;  

	FILE *f = fopen("in.txt", "r");
	int offset = chunk * line_size * (process_rank - 1); 
	
	fseek(f, offset, SEEK_SET);
	
	int read;
	int sumn = 0; 
 
   	for(int i =0; i < chunk; i++){
            fscanf(f, "%d", &read);
            if(read == number) sumn++;
        }
	 
	MPI_Send(&sumn, 1, MPI_INT, MASTER_RANK, 0, MPI_COMM_WORLD);
}

	
