#include <stdio.h>
#include <omp.h>

/**
 * EX: Construir um programa que consiga informar quantas execucoes aconteceram em cada thread
 * */ 

int main(void){

    omp_set_num_threads(6); // setting the number of threads of the system
    
    int hash[6] = {0,0,0,0,0,0}; // shared among the threads 

    #pragma omp parallel 
    {    
        int t_num = omp_get_thread_num(); 

        #pragma omp for  // each iteration of the for loop will be distributed in one thread 
        for(int i = 0; i < 100; i++){
            hash[t_num]++; 
            printf("Thread: %d, i: %d\n", t_num, i); 
        }
    } /* End of parallel area */

    for(int i = 0; i < 6; i++){
        printf("Thread: %d, executions: %d", i, hash[i]);
    }
    return 0;
}