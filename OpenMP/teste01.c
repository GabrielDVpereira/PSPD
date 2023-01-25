#include <stdio.h>
#include <omp.h>

int main(void){

    omp_set_num_threads(6); // setting the number of threads of the system

    printf("Hello \n");

    #pragma omp parallel {
        printf("Thread: %d\n", omp_get_thread_num()); // getting the number of the current thread 
    } /* End of parallel area */

    printf("Hello after\n");
}