#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define TAM 9
int main(void) {
        int a[TAM], b[TAM], c[TAM];

        for(int i = 0; i < TAM; i++){
                a[i] = rand() % 11;
                b[i] = rand() % 5;
        }

        omp_set_num_threads(3);

        #pragma omp parallel
        {
                int num_of_threads = omp_get_max_threads();
                int t_num = omp_get_thread_num();

                int offset_size = TAM / num_of_threads;
                int offset = offset_size * t_num;

                for(int i = offset; i < offset + offset_size; i++){
                        c[i] = a[i] + b[i];
                        printf("c[%d]=%d \n", i, c[i]);
                }
        }
}