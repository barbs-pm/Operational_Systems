
/*_________________~<>~__________________*
*                                        *
*  UNIVERSIDADE FEDERAL DA FRONTEIRA SUL *
*  ALUNA: BÁRBARA PEGORARO MARKUS        *
*  PROFESSOR: MARCO AURELIO S            *
*  DISCIPLINA: SISTEMAS OPERACIONAIS     *
*                                        *
*  gcc TP3.c -o tp3 -lpthread -lrt       *
*__________________~<>~__________________*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define THMAX 6
#define GLOBAL 30

int glob;
sem_t s;
int control;

pthread_mutex_t lock     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond     = PTHREAD_COND_INITIALIZER;

void ast(){

	for(int i = 0; i < 30; i++)
		printf("*");

	printf("\n");

}

void *semaforo(void *data){

	int id;
	id = *((int *) data);
	free(data);

	while(1){

		if(glob == GLOBAL)
			pthread_exit(NULL);

        // semáforo
		if(control%THMAX == id){
			sem_wait(&s); 
			glob++;
			printf("Thread ID %d | Global = %d\n", id, glob);
			control++;
			sem_post(&s); 
		}

	}

	pthread_exit(NULL);
}

void *variavelCondicional (void *data) {

    int id;
	id = *((int *) data);
	free(data);

	while(1){

		if(glob == GLOBAL)
			pthread_exit(NULL);

		if(control%THMAX == id){
			
			pthread_mutex_lock( &lock );
			glob++;
			printf("Thread ID %d | Global = %d\n", id, glob);
			control++;
			pthread_cond_signal(&cond);
			pthread_mutex_unlock( &lock );

		}

	}

	pthread_exit(NULL);

}

void criaThreads (int opc) {

    int i;
	glob    = 0;
	control = 0;

	if (opc == 2) {
	
		pthread_mutex_init(&lock, NULL);
		pthread_cond_init(&cond, NULL);
	}
	else 
		sem_init(&s, 0, 1); // inicializa o semáforo com valor 1
	
	pthread_t Thread[THMAX];

	for(i = 0; i < THMAX; i++){

		int *j = malloc(sizeof(int));
		*j = i;
		
		pthread_create(&Thread[i], NULL, (opc == 1 ? &semaforo : &variavelCondicional), (void *)j);

	}

	for(i = 0; i < THMAX; i++)
		pthread_join(Thread[i], NULL);

}

void menu(){

	ast();
	printf("Escolha a operação:\n\n");
	printf("1- SEMÁFORO\n");
	printf("2- VARIÁVEIS CONDICIONAIS\n");
	printf("3- SAIR\n");
	ast();

}

void escolha(int opc, char str[20]){

	switch(opc){
		case 1:

			system("clear");
			criaThreads(opc);
			printf("\n");
			break;

		case 2:

			system("clear");
			criaThreads(opc);
			printf("\n");
			break;

		case 3:
			exit(0);

		default:
			system("clear");
			printf("Operação Inválida, tente novamente\n");
			break;
	}
}

int main(){

	int loop;
    system("clear");

	do { 

		menu();
		char str[20];
		int opc;
		scanf("%d", &opc);
		fgets(str, 20, stdin);
		escolha(opc,str);
		loop = opc;

	} while(loop);

}