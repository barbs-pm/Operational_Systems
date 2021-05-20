/*______________________~<>~______________________*
*                                                 *
*  UNIVERSIDADE FEDERAL DA FRONTEIRA SUL          *
*  BÁRBARA PEGORARO MARKUS                        *
*                                                 *
*  Problema do Barbeiro Adormecido com o uso de   *
*  semáforos                                      *
*                                                 *
*  gcc SleepingBarber.c -o tp5 -lpthread -lrt     *      
*______________________~<>~______________________*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define numCadeiras 5

int numFregues = numCadeiras + 1;
int feito = 0;
int freguesAux = 0;

pthread_t ThreadBarbeiro;
pthread_t ThreadFregues[numCadeiras+1];

// controla cadeiras disponiveis
sem_t cadeiras;   

// só pode sentar um por vez
sem_t cadeiraBarbeiro;

// fará o barbeiro dormir até chegar um Freguês
sem_t barbeiroDormindo;

// fará o freguês esperar o barbeiro de cortar o cabelo
sem_t freguesSentado;

// inicialização dos semáforos
void inicializa () {

    sem_init(&cadeiras, 0, numCadeiras);
    sem_init(&cadeiraBarbeiro, 0, 1);
    sem_init(&barbeiroDormindo, 0, 0);
    sem_init(&freguesSentado, 0, 0);

}

/*processo do freguês de sentar na cadeira caso haja espaço
acordar o barbeiro, sentar na cadeira de cortar o cabelo
e ir embora da barbearia*/ 

void *fregues(void *number) {

    int num = *(int *)number;

    sleep(5);
    printf("Freguês %d chegando na barbearia.\n", num);

    freguesAux++;

    // caso haja cadeiras disponiveis na "sala de espera"
    if (freguesAux <= numCadeiras) {

        // esperar vagar uma cadeira na sala de espera
        sem_wait(&cadeiras);
        printf("Freguês %d sentando na cadeira.\n", num);

        // esperar a cadeira do barbeiro estar livre
        sem_wait(&cadeiraBarbeiro);

        // a cadeira esta livre entao liberar o lugar
        // na sala de espera
        sem_post(&cadeiras);

        // acordar the barbeiro...
        sem_post(&barbeiroDormindo);

        // esperar o corte de cabelo
        sem_wait(&freguesSentado);

        // liberar a cadeira do barbeiro
        sem_post(&cadeiraBarbeiro);

        printf("Freguês %d saindo da barbearia.\n", num);
    
    }
    else    
        printf("Não há mais vagas e o freguês %d está saindo da barbearia.\n", num);    
    
}

/*
processo de "adormecer" o barbeiro até ter um freguês
caso tiver um freguês cortando o cabelo liberar ele
da cadeira do barbeiro. Caso nao tenha mais fregueses
encerrar o processo */

void *barbeiro(void *data) {

    while (!feito) {

        // Dormir ate um fregues o acordar
        printf("O barbeiro está dormindo\n");
        sem_wait(&barbeiroDormindo);

        // enquanto tiver freguês
        if (!feito) {

            printf("O barbeiro está cortando o cabelo\n");
            sleep(3);
            
            // Liberar o fregues após cortar
            freguesAux --;
            sem_post(&freguesSentado);

        }
        else 
            printf("Acabaram todos os fregueses.\n");
    }
}

void criaThreads() {

    int i = 0;
    int Number[numFregues];

    // criar o barbeiro
    pthread_create(&ThreadBarbeiro, NULL, barbeiro, NULL);

    // ID dos fregueses
    for (i = 0; i < numFregues; i++)
        Number[i] = i;

    // criar os fregueses conforme ID populado no vetor
    for (i = 0; i < numFregues; i++) 
        pthread_create(&ThreadFregues[i], NULL, fregues, (void *)&Number[i]);

}

int main(int argc, char *argv[]) {

    inicializa();
    criaThreads();

    // join nas threads dos fregueses
    for (int i = 0; i < numFregues; i++)
        pthread_join(ThreadFregues[i],NULL);

    feito = 1;
    sem_post(&barbeiroDormindo);  // liberar o barbeiro
    pthread_join(ThreadBarbeiro,NULL); // mensagem q acabaram os fregueses

}


