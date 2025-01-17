#include <stdio.h>
#include <omp.h>

int main(void){

    omp_set_num_threads(6); // setting the number of threads of the system

    #pragma omp parallel {
        
        #pragma omp for  // each iteration of the for loop will be distributed in one thread 
        for(int i = 0; i < 3; i++){
            printf("Thread: %d, i: %d\n", omp_get_thread_num(), i); 
        }
    } /* End of parallel area */
    return 0;
}