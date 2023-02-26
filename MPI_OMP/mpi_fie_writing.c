#include <stdio.h>
#include <mpi.h>
#include <string.h>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    MPI_File fh;
    MPI_File_open(MPI_COMM_WORLD, "output.txt", 
                  MPI_MODE_CREATE | MPI_MODE_WRONLY, 
                  MPI_INFO_NULL, &fh);

    char message[100];
    sprintf(message, "Hello from processor %s, rank %d out of %d processors\n",
            processor_name, rank, size);
    int message_len = strlen(message);
    MPI_File_write_at(fh, rank * message_len, message, 
                      message_len, MPI_CHAR, MPI_STATUS_IGNORE);
    MPI_File_close(&fh);

    MPI_Finalize();
    return 0;
}
