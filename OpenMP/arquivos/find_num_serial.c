#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#define MAX 100000000

int main(void){
    int numberToFind = 3;
    int sum = 0;

    #pragma omp parallel{
        int threadNum = omp_thread_num();
        int threadQtt = omp_get_num_threads();
        int lineSize = 9;
        FILE *fd = fopen("in.txt", "r");
        int offset = threadNum*lineSize*MAX/threadQtt; // Cada thread tem que ler uma parte do arquivo, começando do final da ultima thread
        fseek(fd, offset, SEEK_SET);
        int lido;
        fscanf(fd, "%d", &lido);
        fprintf(stdout, "Thread %d leu: %d\n", omp_thread_num(), lido);
    }

    return 0;
}