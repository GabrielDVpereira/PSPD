#include "word_counter.h"


#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <float.h>
#include <math.h>
#include <string.h>


char * read_file(char*);
char* copyString(char s[], int);
CLIENT* create_client(char *host);
pthread_t* start_workers(int, char *[]);
void* worker_runner(void *);
void wait_workers(int, pthread_t *);

sem_t mutex;

typedef struct {
	char * host; 
	params op; 
} WorkerArgs; 


result res = { 0, 0 };

int
main (int argc, char *argv[])
{
    if (argc < 4) {
        printf ("usage: %s word file host1 host2\n", argv[0]);
        exit (1);
    }


	int nWorkers = argc - 3;
	pthread_t *tids = start_workers(nWorkers, argv + 1);
	wait_workers(nWorkers, tids);	

    printf("Ocorrencias da palavra %s: %d, Numero total de palavras no arquivo: %d \n", argv[1] ,res.occurrences, res.count);
    return 0;
}


pthread_t* start_workers(int nWorkers, char *argv[]) {
    printf("start worder \n");  
	pthread_t *tids = malloc(nWorkers * sizeof(pthread_t));
    WorkerArgs *workersArgs = malloc(nWorkers * sizeof(WorkerArgs));

	char * word = argv[0]; 
	char * filename = argv[1]; 
 
	char *buffer = read_file(filename);
	int buffer_len = strlen(buffer); 
	int word_len = strlen(word); 
        
	if(workersArgs == NULL || tids == NULL || buffer == NULL) {
        printf("Memomry error\n");
        exit(0);
    }


	int workerDataLen = buffer_len / nWorkers; 
      
	sem_init(&mutex, 0, 1);				
	for (int i = 0; i < nWorkers; i++) {
		workersArgs[i].host = argv[i + 2]; 

		int offset = i*workerDataLen; 

		workersArgs[i].op.word.word_val = word; 
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
