#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define READERS 20       // Quantidade de leitores
#define WRITERS 3        // Quantidade de escritores

sem_t mutex_writer;         
sem_t mutex_reader;      
int reader_counter;                  // Total de usuários

void read_data_base(){
    int readingTime = rand() % 3;
    
    printf ("Usuário lendo. Total de %d usuários lendo agora.\n", reader_counter);
    usleep(readingTime * 1000000);
}

void use_data_read(){
    int useTime = rand() % 20;

    printf ("Usuário usando dados.\n");
    usleep(useTime * 1000000);
}

void think_up_data(){
    int thinkTime = rand() % 10;

    printf ("Usuário pensando no que irá escrever.\n");
    usleep(thinkTime * 1000000);
}

void write_data_base(){
    int writeTime = rand() % 6;

    printf ("Escritor escrevendo.\n");
    usleep(writeTime * 1000000);
}

void reader(){
    while(1){
        sem_wait(&mutex_reader);              //down(&mutex_reader); garante acesso exclusivo a variavel reader_counter
        reader_counter = reader_counter + 1;                          //um novo leitor

        if(reader_counter == 1) sem_wait(&mutex_writer);  //caso este seja o primeiro leitor...
        sem_post(&mutex_reader);              //up(&mutex_reader); libera o acesso a variavel reader_counter

        read_data_base();                     //le a base de dados
        sem_wait(&mutex_reader);              //down(&mutex_reader); garante acesso exclusivo a variavel reader_counter
        reader_counter = reader_counter - 1;                          //um leitor a menos...

        if(reader_counter == 0) sem_post(&mutex_writer);  //caso este seja o ultimo leitor...
        sem_post(&mutex_reader);              //up(&mutex_reader); libera o acesso a variavel reader_counter

        use_data_read();                      //utiliza as informacoes
    }
}

void writer(){
    while (1){
        think_up_data();            //pensa em informações para adicionar a base de dados
        sem_wait(&mutex_writer);    //down(&mutex_writer); garante acesso exclusivo a base de dados
        write_data_base();          //escreve novas informacoes na base de dados

        sem_post(&mutex_writer);    //up(&mutex_writer); libera o acesso a base
    }
}

int main (){
    pthread_t writerthreads[WRITERS], readerthreads[READERS];

    // Declaração dos semáforos
    sem_init(&mutex_writer, 0, 1);
    sem_init(&mutex_reader, 0, 1);

    // Criação das threads
    for (int i = 0; i < WRITERS; i++){
        pthread_create( &writerthreads[i], NULL,(void *) writer, NULL);
    }
    for (int i = 0; i < READERS; i++){
        pthread_create( &readerthreads[i], NULL,(void *) reader, NULL);
    }

    // Aguarda a finalização das threads
    for (int i = 0; i < WRITERS; i++){
        pthread_join(writerthreads[i], NULL);
    }
    for (int i = 0; i < READERS; i++){
        pthread_join(readerthreads[i], NULL);
    }

    return 0;
}
