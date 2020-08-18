#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>

//Variáveis Globais
sem_t mutex_envia;
sem_t mutex_recebe;
int buffer;

void envia() {
	while(true) {
		sem_wait(&mutex_envia);
		buffer = rand() % 100;
		sem_post(&mutex_recebe);
	}
}

void recebe() {
	while(true) {
		sem_wait(&mutex_recebe);
		printf("%d\n", buffer);
		sem_post(&mutex_envia);
	}
}

int main() {
	//Threads
	pthread_t thread_envia;
	pthread_t thread_recebe;

	//Inicializando os semáforos
	sem_init(&mutex_envia, 0, 1);
	sem_init(&mutex_recebe, 0, 0);

	//Criando as threads
	pthread_create(&thread_envia, NULL, (void *) envia, NULL);
	pthread_create(&thread_recebe, NULL, (void *) recebe, NULL);

	pthread_join(thread_envia, NULL);
	pthread_join(thread_recebe, NULL);

	//Destruindo os semáforos
	sem_destroy(&mutex_envia);
	sem_destroy(&mutex_recebe);

	return 0;
}