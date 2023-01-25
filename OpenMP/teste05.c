#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define MAX_SIZE 100000000

/**
 * Criar código que inicialize de forma randomica um vetor de 100milhoes de posicoes e conte quantos zeros ele possui
*/

void init_arr(int * arr){
    for(int i =0; i < MAX_SIZE; i++)
        arr[i] = rand() % 5;  // valores de 0 a 5
}

int zero_counter(int * arr, int size){
    int count = 0;
    for(int i = 0; i < size; i++)
        if(arr[i] == 0) count ++;
    return count;
}

int main(void){

    omp_set_num_threads(6);

    int * arr = (int*)malloc(sizeof(int) * MAX_SIZE);

    init_arr(arr);

    int count = 0;

    #pragma omp parallel
    {
        int t_num = omp_get_thread_num();
        int num_of_threads = omp_get_max_threads();
        int offset_size = MAX_SIZE / num_of_threads;
        int offset = offset_size * t_num;
        count+= zero_counter(arr + offset, offset_size);
    }

    printf("Number of zeros: %d\n", count);
    free(arr);

    return 0;
}