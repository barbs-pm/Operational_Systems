/**********************************************************************************
	UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
    Autora:     Bárbara Pegoraro Markus
    Definição:  Duas thread competem por 2 mutex gerando um deadlock. Para resolver isso
                foi adicionada a chamada da função pthread_mutex_trylock e sleep para que 
                o trylock tentasse ganhar o recurso após esse tempo
    Data:       21/03/2020
    Fontes:     https://docs.oracle.com/cd/E36784_01/html/E36874/pthread-mutex-trylock-3c.html
                https://www.ibm.com/support/knowledgecenter/en/SSLTBW_2.4.0/com.ibm.zos.v2r4.bpxbd00/ptmtylk.htm
                https://stackoverflow.com/questions/14005229/pthread-mutex-trylock-in-linux
**********************************************************************************/
 
// inclusão de bibliotecas
#include <pthread.h>
#include <unistd.h> 
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

// declaração das funções
void *thread1(void *data);
void *thread2(void *data);

// declaração das threads e dos mutex
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER, mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_t Thread1, Thread2;

// Função main
int main(void) {

    // criação das threads
    pthread_create(&Thread1, NULL, thread1, NULL);
    pthread_create(&Thread2, NULL, thread2, NULL);

    pthread_join(Thread1, NULL);
    printf("\nThread 1 %ld retornou\n", Thread1);

    pthread_join(Thread2, NULL);
    printf("\nThread 2 %ld retornou\n", Thread2);

    printf("\n*************************************************************\n");
    printf("Deu tudo certo, sem deadlock, meu processador está a salvo :)\n");

    return 0;
}

/***********************************************************************************/

void *thread1(void *data){
    unsigned long i;

    // irá tentar lockar o mutex1
    if(pthread_mutex_trylock(&mutex1) == 0){

        printf("Thread: %ld conseguiu lockar o mutex1.\n", pthread_self());
        sleep(rand()%2);

        for(i=0; i< 10000000; ++i); // just for wasting some time

        // irá tentar lockar o mutex2
        if(pthread_mutex_trylock(&mutex2) == 0){

            printf("Thread: %ld conseguiu lockar o mutex2.\n", pthread_self());          
            sleep(rand()%2);

            for(i=0; i< 10000000; ++i); // just for wasting some time
            
            // unlock o mutex
            pthread_mutex_unlock(&mutex2);   

        } 
        // se já estiver lockado
        else {
            // unlock o mutex
    		printf("Thread: %ld não conseguiu lockar o mutex2.\n", pthread_self()); 
            pthread_mutex_unlock(&mutex2);
   		}
        
        // unlock o mutex
        pthread_mutex_unlock(&mutex1);

    } 
    // se já estiver lockado
    else {
        // unlock o mutex
    	printf("\nThread: %ld não conseguiu lockar o mutex1.\n", pthread_self()); 
        pthread_mutex_unlock(&mutex1);
    }

  	pthread_exit(NULL);
}

/***********************************************************************************/

void *thread2(void *data){
    unsigned long i;

    // irá tentar lockar o mutex2
    if(pthread_mutex_trylock(&mutex2) == 0){

        printf("Thread: %ld conseguiu lockar o mutex2.\n", pthread_self());
        sleep(rand()%2);

        for(i=0; i< 10000000; ++i); // just for wasting some time

        // irá tentar lockar o mutex1
        if(pthread_mutex_trylock(&mutex1) == 0){

        	printf("Thread: %ld conseguiu lockar o mutex1.\n", pthread_self());          
            sleep(rand()%2);

            for(i=0; i< 10000000; ++i); // just for wasting some time
            
            // unlock o mutex
            pthread_mutex_unlock(&mutex1);

        // se já estiver lockado
        } else {
            // unlock o mutex
        	printf("Thread: %ld não conseguiu lockar o mutex1.\n", pthread_self()); 
            pthread_mutex_unlock(&mutex1);
        }

        // unlock o mutex
        pthread_mutex_unlock(&mutex2);

    } 
    // se já estiver lockado
    else {
        // unlock o mutex
        printf("Thread: %ld não conseguiu lockar o mutex2.\n", pthread_self()); 
        pthread_mutex_unlock(&mutex2);
    }

  	pthread_exit(NULL);
}
