/*______________________~<>~______________________*
*                                                 *
*  UNIVERSIDADE FEDERAL DA FRONTEIRA SUL          *
*  BÁRBARA PEGORARO MARKUS                        *
*                                                 *
*  gcc TP4.c -o tp4 -lpthread -lrt -lm            *      
*______________________~<>~______________________*/

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <math.h>

#define N_INTERACOES 10
#define MAX_THREADS 9
#define TAMANHO_VETOR 10

double vetor_velho[TAMANHO_VETOR];
double vetor_novo[TAMANHO_VETOR];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
sem_t semaforo[MAX_THREADS];
pthread_t ThreadID[MAX_THREADS];

static pthread_barrier_t barreira1;
static pthread_barrier_t barreira2;

void inicializa() {

	// inicializa barreiras
	pthread_barrier_init(&barreira1, NULL, MAX_THREADS);
	pthread_barrier_init(&barreira2, NULL, MAX_THREADS);
	
	// inicializa semaforo
	for(int i = 0; i < MAX_THREADS; i++) 
		sem_init(&semaforo[i], 0, 0);
	
	//seta valores zerados para o vetor
	for(int i = 0; i < (TAMANHO_VETOR - 1); i++) {
		vetor_velho[i] = 0;
		vetor_novo[i] = 0;
	}
	
	// ultima posição do vetor recebe 1
	vetor_velho[TAMANHO_VETOR - 1] = 1;
	vetor_novo[TAMANHO_VETOR - 1] = 1; 

}

// média entre os valores dos vizinhos à esquerda e à direita
int fatia(int fatia) {

	int divisao, aux;

	divisao = TAMANHO_VETOR/MAX_THREADS;
	aux = fatia * divisao + 1;
	
	return aux;

}

int fatiaFinal(int fatia, int valorAtual) {

	int resto = TAMANHO_VETOR % MAX_THREADS;
	
	// se for a ultima thread
	if(fatia == MAX_THREADS - 1) {
		if(resto != 0) 
			return TAMANHO_VETOR - 1;		
		else //no caso em que a última fatia seja menor que as demais
			return TAMANHO_VETOR / MAX_THREADS - 2 + valorAtual;
		
	}
	else 
		return TAMANHO_VETOR / MAX_THREADS + valorAtual;
	
}

// imprimir os valores do vetor
void imprime(double vetor[TAMANHO_VETOR]) {

	for(int i = 0; i < TAMANHO_VETOR; i++) 
		printf("%lf | ", vetor[i]);
	
	printf("\n");
	
}

void *swapVetores(void* data) {

	int i = *((int*) data);

	for(int k = 0; k < N_INTERACOES; k++) {

		// barreira em espera
		pthread_barrier_wait(&barreira1);

	    // calcula-se a seguinte recorrência para todos os elementos da posição 1 a N
		for(int j = fatia(i); j < fatiaFinal(i, fatia(i)); j++)
			vetor_novo[j] = (vetor_velho[j-1] + vetor_velho[j+1])/2.0;

		// apenas uma das threads deve fazer o swap entre os dois vetores
		if(pthread_barrier_wait(&barreira2) != 0) {

			imprime(vetor_novo);

			// faz o swap de fato
			for(int m = 1; m < (TAMANHO_VETOR - 1); m++) 
				vetor_velho[m] = vetor_novo[m];
			
		}

	}
	
}

int main(void) {
	
	inicializa();
	
	// criar as threads
	for(int i = 0; i < MAX_THREADS; i++) {	

		int *data = malloc(sizeof(int));
		*data = i;
		
		pthread_create(&ThreadID[i], NULL, swapVetores, (void *) data);	

	}

	//join nas thread
	for(int i = 0; i < MAX_THREADS; i++) 
		pthread_join(ThreadID[i], NULL);
	
	return 0;

}
