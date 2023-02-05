#include <stdio.h>
#include <omp.h>

#define MAX 1000000

int main(void){
    int sumn = 0; 
    int lido; 
    int n = 3; 

    #pragma omp parallel for reduction(+:sumn, lido)
    for(int i = 0; i < MAX; i++){
        scanf("%d", &lido);
        if(lido == n) sumn++;
    }

    printf("Encontramos %d ocorrencias de %d", sumn, n);

    return 0; 
}