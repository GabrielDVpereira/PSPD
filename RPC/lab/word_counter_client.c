#include "word_counter.h"


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <float.h>
#include <math.h>
#include <string.h>



typedef struct {
	char * host; 
	params op; 
} WorkerArgs; 


result res = { 0, 0 };

typedef struct {
	char filename[256]; 
	char word[256]; 
	int nWorkers; 
	char hosts[2][256]; 
} Inputs;

char * read_file(char*);
char* copyString(char s[], int);
CLIENT* create_client(char *host);
pthread_t* start_workers(Inputs);
void* worker_runner(void *);
void wait_workers(int, pthread_t *);
void get_user_input(Inputs* inputs); 

sem_t mutex;

int
main (int argc, char *argv[])
{

	Inputs inputs; 
	get_user_input(&inputs); 
	pthread_t *tids = start_workers(inputs);
	wait_workers(inputs.nWorkers, tids);	

    printf("Ocorrencias da palavra %s: %d, Numero total de palavras no arquivo: %d \n", inputs.word ,res.occurrences, res.count);
    return 0;
}


void get_user_input(Inputs* inputs){
	printf("Digite o nome do arquivo que deseja utilizar: \n"); 
	scanf("%s" , inputs->filename);

	printf("Digite o nome da palavra que seja saber a ocorrencia: \n"); 
	scanf("%s" , inputs->word);

	printf("Quantidade de workers (1 ou 2): \n"); 
	scanf("%d", &inputs->nWorkers); 

	if(inputs->nWorkers > 2 || inputs->nWorkers < 1){
		printf("Quantidade de workers deve ser entre 1 e 2"); 
	}

	for(int i = 0; i < inputs->nWorkers; i++){
		char temp[256]; 
		printf("Nome do host numero %d\n", i); 
		scanf("%s" , temp);
		strcpy(inputs->hosts[i], temp); 
	}
}


pthread_t* start_workers(Inputs inputs) {
    printf("start worder \n");  
	pthread_t *tids = malloc(inputs.nWorkers * sizeof(pthread_t));
    WorkerArgs *workersArgs = malloc(inputs.nWorkers * sizeof(WorkerArgs));

	char *buffer = read_file(inputs.filename);
	int buffer_len = strlen(buffer); 
	int word_len = strlen(inputs.word); 
        
	if(workersArgs == NULL || tids == NULL || buffer == NULL) {
        printf("Memomry error\n");
        exit(0);
    }


	int workerDataLen = buffer_len / inputs.nWorkers; 
      
	sem_init(&mutex, 0, 1);				
	for (int i = 0; i < inputs.nWorkers; i++) {
		workersArgs[i].host = inputs.hosts[i]; 

		int offset = i*workerDataLen; 

		workersArgs[i].op.word.word_val = inputs.word; 
		workersArgs[i].op.data.data_val = buffer + offset; 
		workersArgs[i].op.word.word_len = word_len; 
		workersArgs[i].op.data.data_len = workerDataLen; 
		
		pthread_create(&tids[i], NULL, &worker_runner, (void *)&workersArgs[i]);
		
	}

	return tids;
}


void* worker_runner(void *arg) {
	WorkerArgs *args = (WorkerArgs*) arg;
	result *tmp;
	CLIENT *clnt = create_client(args->host);
	tmp = count_word_100(&args->op, clnt);


	sem_wait(&mutex);


	res.count += tmp->count;
	res.occurrences += tmp->occurrences;

	sem_post(&mutex);
}

void wait_workers(int nWorkers, pthread_t *tids) {
	for (int i = 0; i < nWorkers; i++) {
		pthread_join(tids[i], NULL);
	}
}

char* copyString(char s[], int len)
{
    char* s2;
    s2 = (char*)malloc(len);
    strcpy(s2, s);
    return (char*)s2;
}


char *read_file(char *filename)
{
  FILE *file;

  file = fopen(filename, "r");
  if (file == NULL) return NULL;
  fseek(file, 0, SEEK_END);
  int length = ftell(file);
  fseek(file, 0, SEEK_SET);

  char *string = malloc(sizeof(char) * (length+1));
  char c;

  int i = 0;

  fread(string, sizeof(char), length, file);

  fclose(file);
  return string;
}

CLIENT* create_client(char *host) {
	CLIENT *clnt = clnt_create(host, PROG, VERSAO, "tcp");
	if (clnt == NULL) {
        	clnt_pcreateerror (host);
                exit (1);
        }
	return clnt;
}
