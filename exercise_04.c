/*
Scrivere un programma in C in cui il main genera 2 thread secondari (Thread A e Thread B)
Tutti i Thread condividono due variabili (VarA e VarB), inizializzate a 0.

Il Main Thread, ciclicamente, somma le variabili condivise se e solo se il valore di entrambe le variabili è maggiore di 0.
Ogni volta che il main thread esegue la somma, resetta le variabili VarA e VarB al valore 0.
Il Main Thread ad ogni ciclo va in sleep per 1 secondo

Il Thread A, ciclicamente, assegna alla variabile VarA un valore random tra 0 e 5.
Il Thread B, ciclicamente, assegna alla variabile VarB un valore pari all'iterazione eseguita 
(es. 1 al primo ciclo, 2 al secondo ciclo, 3 al terzo ciclo, e cosi via).
Il Thread B ad ogni ciclo va in sleep per 2 secondi.

Il Main Thread dopo 10 cicli attende la terminazione di tutti i Thread e termina esso stesso.
I Thread A e B dopo 5 cicli ciascuno terminano.

Utilizzare Il meccanismo dei pthread_mutex per regolare l'accesso in sezione critica. 
NOTA: il Main Thread resetta solo le varibili VarA e VarB, non i contatori dei cicli.
*/

// LIBRERIE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// PROTOTIPI
void *thread_A_function(void *args);
void *thread_B_function(void *args);

// COSTANTI, VARIABILI GLOBALI
pthread_mutex_t mutex;
#define NUM_CYCLES_THREAD 5
#define NUM_CYCLES_MAIN_THREAD 10
int VarA;
int VarB;

// MAIN
int main(void){
	VarA = 0;
	VarB = 0;
	int result;
	int i;
	int somma;
	pthread_t thread_A;
	pthread_t thread_B;
	
	result = pthread_mutex_init(&mutex, NULL);
	if(result != 0){
		perror("pthread_init");
		exit(EXIT_FAILURE);
	}
	
	result = pthread_create(&(thread_A), NULL, thread_A_function, NULL);
	if(result != 0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	result = pthread_create(&(thread_B), NULL, thread_B_function, NULL);
	if(result != 0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}	
	
	for(i=0; i<NUM_CYCLES_MAIN_THREAD; i++){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}	
		if(VarA>0 && VarB>0){
			somma = VarA + VarB;
			printf("\nLa SOMMA delle Due Variabili Globali e': %d\n",somma);
			
			VarA = 0;
			VarB = 0;
		}
		
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}		
		sleep(1);				
	}
	
	result = pthread_join(thread_A, NULL);
	if(result != 0){
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}	
	
	result = pthread_join(thread_B, NULL);
	if(result != 0){
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}
	
	result = pthread_mutex_destroy(&mutex);
	if(result != 0){
		perror("pthread_mutex_destroy");
		exit(EXIT_FAILURE);
	}		
}

// FUNZIONI
void *thread_A_function(void *args){
	int result;
	int i;
	int n;
	
	srand(time(NULL));
	
	for(i=0; i<NUM_CYCLES_THREAD; i++){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		
	//  n = (rand()%(max-min+1))+min;
		n = (rand()%(5-0+1)+0);
		VarA = n;
		printf("\nVarA = %d",VarA);
		
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}
		sleep(2);	
	}
	pthread_exit(NULL);
}

void *thread_B_function(void *args){
	int result;
	int i;
	
	for(i=0; i<NUM_CYCLES_THREAD; i++){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		
		VarB++;
		printf("\nVarB = %d",VarB);
			
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}
		sleep(2);	
	}	
	pthread_exit(NULL);
}