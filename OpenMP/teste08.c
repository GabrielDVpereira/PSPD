#include <stdio.h>
#include <omp.h>
#include <stdlib.h>


/** SOLUCAO COM REDUCTION PARA THREADS ACESSANDO O MESMO ACUMULADOR */
#define TAM 9
int main(void) {
    int a[TAM], b[TAM], c[TAM];

    for(int i = 0; i < TAM; i++){
            a[i] = rand() % 11;
            b[i] = rand() % 5;
    }

    omp_set_num_threads(3);
    int sum = 0; 
    #pragma omp parallel for reduction (+: sum)
    for(int i = offset; i < TAM; i++){
            c[i] = a[i] + b[i];
            sum+=c[i];
    }


    return 0; 
}


/** SOLUCAO COM CRITICAL PARA THREADS ACESSANDO O MESMO ACUMULADOR */
/**  Não tão boa pois deixa blocked cada thread ate uma terminar   */
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

    int sum = 0; 
    #pragma omp parallel for // cada exeucao do for vai ir para uma thread especifica
    for(int i = 0; i < TAM; i++){
            c[i] = a[i] + b[i];

            #pragma omp critical // torna o comando como uma area critica, ou seja processos n pode mexer ao mesmo tempo
            sum+=c[i];
    }


    return 0; 
}


/** SOLUCAO COM ATOMIC PARA THREADS ACESSANDO O MESMO ACUMULADOR */
/**  melhor que o critical pois implementa area critica a nivel de hardware   */
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

    int sum = 0; 
    #pragma omp parallel for
    for(int i = 0; i < TAM; i++){
            c[i] = a[i] + b[i];
            
            #pragma omp atomic // cria area critica a nivel de hardware
            sum+=c[i];

    }


    return 0; 

}


/** SOLUCAO COM ATOMIC COM PRIVATE PARA THREADS ACESSANDO O MESMO ACUMULADOR */
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>

#define TAM 9
int main(void) {
    int a[TAM], b[TAM], c[TAM];
    
    int total = 0; 

    for(int i = 0; i < TAM; i++){
            a[i] = rand() % 11;
            b[i] = rand() % 5;
    }

    omp_set_num_threads(3);

    #pragma omp parallel private(soma_local, i)
    {
        int soma_local = 0; 

        #pragma omp for
        for(int i = 0; i < TAM; i++){
            c[i] = a[i] + b[i];            
            soma_local+=c[i];
        }

        #pragma omp atomic
        total+=soma_local;
    }

    return 0; 
}