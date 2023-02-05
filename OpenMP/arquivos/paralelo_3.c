#include <stdio.h>
#include <omp.h>

#define MAX 1000000

int main(void){
    int n = 3, line_size = 9; 
    int sumn = 0; 

    // para thread vai ler uma parte do arquivo
    #pragma omp parallel reduction(+:sumn)
    {
        FILE *fd = fopen("in.txt", "r"); 
        int lido; 
        int chunk = MAX / omp_get_num_threads();
        int offset = chunk * line_size * omp_get_thread_num();

        fseek(fd, offset, SEEK_SET);

        for(int i =0; i < chunk; i++){
            fscanf(fd, "%d", &lido);
            if(lido == n) sumn++;
        }
    }

    printf("Total de %d's = %d\n", n, sumn);
    return 0; 
}
