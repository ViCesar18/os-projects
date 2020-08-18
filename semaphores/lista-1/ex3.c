/* Includes */
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <semaphore.h>  /* Semaphore */

/* prototype for thread routine */
void ping ( void *ptr);

void pong (void *ptr);

void pang (void *ptr);
/* global vars */
/* semaphores are declared global so they can be accessed 
   in main() and in thread routine,
   here, the semaphore is used as a mutex */
sem_t mutexping;
sem_t mutexpong;
sem_t mutexpang;


int main()
{
    int i[3];
    pthread_t thread_a;
    pthread_t thread_b;
    pthread_t thread_c;
    
    i[0] = 0; /* argument to threads */
    i[1] = 1;
    i[2] = 2;
    
    sem_init(&mutexping, 0, 1);      /* initialize mutex to 1 - binary semaphore */
                                 /* second param = 0 - semaphore is local */
    sem_init(&mutexpong, 0, 0);

    sem_init(&mutexpang, 0, 0);
                                 
    /* Note: you can check if thread has been successfully created by checking return value of
       pthread_create */                                 
    pthread_create (&thread_a, NULL, (void *) &ping, (void *) &i[0]);
    pthread_create (&thread_b, NULL, (void *) &pong, (void *) &i[1]);
    pthread_create (&thread_c, NULL, (void *) &pang, (void *) &i[2]);
    
    pthread_join(thread_a, NULL);
    pthread_join(thread_b, NULL);
    pthread_join(thread_c, NULL);

    sem_destroy(&mutexping); /* destroy semaphore */
    sem_destroy(&mutexpong);      
    sem_destroy(&mutexpang);        
    /* exit */  
    exit(0);
} /* main() */

void ping ( void *ptr )
{
    int x; 
    x = *((int *) ptr);
    while (1){
	sem_wait(&mutexping);
	printf("PING %d\n",x);
	sem_post(&mutexpong);
	}
}

void pong ( void *ptr )
{
    int x;
    x = *((int *) ptr);
    while (1)
	{
		sem_wait(&mutexpong);
		printf("PONG %d\n", x);
		sem_post(&mutexpang);
	}
}

void pang ( void *ptr )
{
    int x;
    x = *((int *) ptr);
    while (1)
    {
        sem_wait(&mutexpang);
        printf("PANG %d\n", x);
        sem_post(&mutexping);
    }
    pthread_exit(0);
}