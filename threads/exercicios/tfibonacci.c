#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int vet[100000];
int n;

void *fibonacci() {
	int termo1 = 0, termo2 = 1, proxTermo;

	for(int i = 0; i <= n; i++) {
		vet[i] = termo1;
		proxTermo = termo1 + termo2;
		termo1 = termo2;
		termo2 = proxTermo;
	}
}

int main() {
	pthread_t t1;
	char *message1 = "Thread 1";
	int tret1;

	scanf("%d", &n);

	tret1 = pthread_create(&t1, NULL, fibonacci, (void *) message1);

	pthread_join(t1, NULL);

	for(int i = 0; i <= n; i++) {
		printf("%d ", vet[i]);
	}
}