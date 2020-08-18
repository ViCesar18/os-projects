#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int int_val[5];
/* threaded routine */
sem_t mutex[5];

void *add_to_value(void *arg)
{
	int inval = (int) arg;
	int i;
	printf ("\nExecutando thread %d ....", (int) inval/2);

	sem_wait(&mutex[inval / 2]);

	for (i = 0; i < 10000; i++)
		int_val[i % 5] += inval; /* !!! changing global array without synchronization */

	if(inval / 2 + 1 > 4) {
		sem_post(&mutex[0]);
	}
	else {
		sem_post(&mutex[inval / 2 + 1]);
	}

	return (NULL);
}

int main()
{
 int i;
 pthread_t thread[5];

 for(i = 0; i < 5; i++) {
 	if(i == 0)
 		sem_init(&mutex[i], 0, 1);
 	else
 		sem_init(&mutex[i], 0, 0);
 }

 /* initialize the data */
 for (i = 0; i < 5; i++)
 	int_val[i] = 0;

 for (i = 0; i < 5; i++)
	pthread_create (&thread[i], NULL, (void *) &add_to_value, (void *) (2 * i));


 /* wait till all threads have finished */
 for (i = 0; i < 5; i++)
	pthread_join(thread[i], NULL);


 /* print the results */
 printf("\nfinal values…\n");
 for (i = 0; i < 5; i++)
 	printf("integer value [%d] = \t %d\n", i, int_val[i]);
 return(0);
}
