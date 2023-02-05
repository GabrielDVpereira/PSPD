#include <stdio.h>
#include <omp.h>
#define MAX 1000000
int main(void){
    int sumn = 0; 
    int lido; 
    int n = 3; 

    FILE *fd; 

    fd = fopen("in.txt", "r"); 

    #pragma omp parallel for reduction(+:sumn, lido)
    for(int i =0; i < MAX; i++){
        fscanf(fd, "%d", &lido);
        if(lido == n) sumn++; 
    }

    printf("Todal de %d's lidos = %d\n", n, sumn);
   return 0;  
}