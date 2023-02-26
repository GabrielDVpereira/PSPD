## 1 - FRACTAL MPI SERIAL

| Processo | Tempo (1000 linhas) | Tempo (5000 linhas) |
|:--------:|---------------------|---------------------|
| 1        | 1.193s              | 16.514s             |
| 2        | 0.897s              | 9.265s              |
| 3        | 0.874s              | 8.744s              |
| 4        | 0.882s              | 6.573s              |

## 2 - FRACTAL MPI PARALELO

| Processo | Tempo (1000 linhas) | Tempo (5000 linhas) |
|:--------:|---------------------|---------------------|
| 1        | 1.284s              | 15.553s             |
| 2        | 1.088s              | 9.082s              |
| 3        | 0.963s              | 9.770s              |
| 4        | 0.880s              | 7.822s              |

## 3 - FRACTAL OMP

| Threads  | Tempo (1000 linhas) | Tempo (5000 linhas) |
|:--------:|---------------------|---------------------|
| 1        | 0.630s              | 13.963s             |
| 2        | 0.361s              | 7.616s              |
| 3        | 0.355s              | 7.923s              |
| 4        | 0.294s              | 5.998s              |


## Análise

De forma geral, a solução envolvendo threads com OMP apresenta a melhor performance para o problema apresentado. Todavia, vale notar que ainda estamos trabalhando com um volume de dados processável em uma única máquina, o que faz a solução com OMP possível de ser executada. Para uma dimensão de dados muito elevada, seria necessário utilizar a programação distribuída para que o processamento seja feito de diversos nós (máquinas) utilizando o MPI. 

Também, nota-se que mesmo para a solução serial, o aumento do número de processos melhora a performance, uma vez que o processamento dos pixels do fractal é feito uma máquina a parte, onde somente a escrita é realizada de forma serial.




