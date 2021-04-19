/**********************************************************************************
  UNIVERSIDADE FEDERAL DA FRONTEIRA SUL
  Autora:     Bárbara Pegoraro Markus
  Definição:  O Jantar dos Filófos: Usar um array para guardar as informações do estado
              do filósofo (pensando, comendo ou com fome). O filósofo só pode mudar
              o estado para comendo se seus dois vizinhos (esquerdo e direito) não estiverem 
              comendo. Se o filósofo está com fome é usado um semaforo pra o atrasar 
              e impedir de pegar o garfo de outro que ja esta sendo usado
  Data:       21/03/2020
  Fontes:     https://www.youtube.com/watch?v=BjrMrw2GM6o
              https://www.youtube.com/watch?v=6ql4YW1LHC8

  gcc jantar_dos_filosofos.c -o jantar -lpthread -lrt
  ./jantar
**********************************************************************************/

// bibliotecas
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h> 
#include <stdlib.h>
#include <time.h>

// estado dos filósofos e posições
#define COMENDO 0
#define FOME 1
#define PENSANDO 2
#define ESQUERDA (numFilosofo + 4) % 5
#define DIREITA  (numFilosofo + 1) % 5
  
int estado[5];
int phil[5] = { 0, 1, 2, 3, 4 };
int aux = 0;

// semaforos
sem_t mutex;
sem_t S[5];
  
void testandoComer(int numFilosofo)
{   
    // se nenhum filosofo vizinho estiver comendo, o estado do filosofo
    // atual pode ser alterado para comendo se estiver com fome
    if (estado[numFilosofo] == FOME && estado[ESQUERDA] != COMENDO && estado[DIREITA] != COMENDO) {
        estado[numFilosofo] = COMENDO;
  
        sleep(2);
        printf("Filósofo %d está comendo com o garfo %d e %d\n", numFilosofo + 1, ESQUERDA + 1, numFilosofo + 1);
  
        // se retornar > 0 irá dar unlock um filósofo com fome liberando para poder comer
        sem_post(&S[numFilosofo]);
    }
}
  
// filósofo pega garfo
void pegarGarfo(int numFilosofo)
{

    // lockar o mutex 
    sem_wait(&mutex);
  
    // setar estado de fome para o filósofo
    estado[numFilosofo] = FOME;
  
    // testar se nenhum vizinho esta comendo
    testandoComer(numFilosofo);
  
    // deslockar o mutex 
    sem_post(&mutex);
  
    // se o filosofo nao pode comer, esperar ser sinalizado
    sem_wait(&S[numFilosofo]);
  
    sleep(1);
}
  
// filósofo larga garfo
void largarGarfo(int numFilosofo)
{

    // lockar o mutex 
    sem_wait(&mutex);
  
    // setar o estado pensando para o filósofo
    estado[numFilosofo] = PENSANDO;
    
    printf("Filósofo %d largou o garfo %d e %d e está pensando\n", numFilosofo + 1, ESQUERDA + 1, numFilosofo + 1);
  
    // testar se nenhum vizinho esta comendo
    testandoComer(ESQUERDA);
    testandoComer(DIREITA);
    
    // deslockar o mutex 
    sem_post(&mutex);
    
}
  
void* filosofo(void* num)
{

    while (aux < 5) {
        
        aux++;

        int* i = num;
  
        sleep(1);
        pegarGarfo(*i);
        sleep(0);
        largarGarfo(*i);
  
    }

}
  
int main()
{
  
    int i;
    pthread_t thread_id[5];
  
    // inicializar os semáforos
    sem_init(&mutex, 0, 1);
  
    for (i = 0; i < 5; i++)
        sem_init(&S[i], 0, 0);
  
    // criar as threads (filósofos)
    for (i = 0; i < 5; i++) 
        pthread_create(&thread_id[i], NULL, filosofo, &phil[i]);

    printf("Todos os filósofos estão pensando\n\n");
  
    for (i = 0; i < 5; i++)
        pthread_join(thread_id[i], NULL);

    if (aux = 5) {
        printf("\nTodos os 5 filósofos terminaram de comer\n\n");
    }        
}