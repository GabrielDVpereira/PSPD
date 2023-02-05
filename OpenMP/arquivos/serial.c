#include <stdio.h>

#define MAX 1000000

int main(void) {
    int sumn=0;
    int lido;
    int n=3;

    for (int i=0;i<MAX;i++) {
        scanf("%d", &lido);
        
        if (lido == n) sumn++;
    } 
    printf("Encontramos %d ocorrências de %d\n" , sumn, n);
    return 0;
} 