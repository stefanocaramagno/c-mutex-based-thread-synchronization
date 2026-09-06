/*
 * Scrivere un programma in C che permetta la raccolta dati
 * da una serie di sensori (3 sensori).
 * 
 * Ogni sensore è rappresentato da un thread.
 * - Il primo sensore periodicamente e ogni secondo monitora
 *   la temperatura di una stanza.
 * - Il secondo sensore, periodicamente e ogni secondo, monitora
 *   l'umidità di una stanza.
 * - Il terzo sensore, periodicamente e ogni secondo, monitora
 *   il consumo energetico.
 * 
 * Tutti i sensori inviano i propri valori al
 * main thread che si occuperà di stampare a video
 * i valori dei tre sensori ogni 2 secondi.
 * 
 * Dopo 20 secondi di esecuzione tutti i thread sensori
 * imposteranno il valore del sensore a -1 per comunicare 
 * al main thread che hanno terminato.
 *
 * Il main thread, quando leggerà che il valore di
 * tutti e tre i sensori è uguale a -1 termina.
 * 
 * Utilizzare thread/mutex 
 * per l'implementazione del programma.
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
void *thread_C_function(void *args);

// COSTANTI, VARIABILI GLOBALI
pthread_mutex_t mutex;
int temperatura;
int umidita;
int consumo_energetico;

// MAIN
int main(void){
	int result;
	pthread_t thread_A;
	pthread_t thread_B;
	pthread_t thread_C;	
	
	result = pthread_mutex_init(&mutex, NULL);
	if(result != 0){
		perror("pthread_mutex_init");
		exit(EXIT_FAILURE);
	}
	
	result = pthread_create(&(thread_A),NULL,thread_A_function,NULL);
	if(result != 0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	result = pthread_create(&(thread_B),NULL,thread_B_function,NULL);
	if(result != 0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}	
	
	result = pthread_create(&(thread_C),NULL,thread_C_function,NULL);
	if(result != 0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}		

	while(1){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}	
		if(temperatura == -1 && umidita == -1 && consumo_energetico == -1){
			printf("\nTemperatura: %d",	temperatura);
	    	printf("\nUmidita: %d",umidita);
		    printf("\nConsumo Energetico: %d\n",consumo_energetico);
			exit(EXIT_SUCCESS);
		}
				
		printf("\nTemperatura: %d",	temperatura);
		printf("\nUmidita: %d",umidita);
		printf("\nConsumo Energetico: %d\n",consumo_energetico);
	
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}
		sleep(2);				
	}
	
	result = pthread_join(thread_A,NULL);
	if(result != 0){
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}	
	
	result = pthread_join(thread_B,NULL);
	if(result != 0){
		perror("pthread_join");
		exit(EXIT_FAILURE);
	}	
		
	result = pthread_join(thread_B,NULL);
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

void *thread_A_function(void *args){
	int result;
	int n;
	
	srand(pthread_self());
	
	for(int i=0; i<20; i++){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		
		n = rand();
		temperatura = n;
	
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}
		sleep(1);				
	}
	
	temperatura = -1;

	pthread_exit(NULL);		
}

void *thread_B_function(void *args){
	int result;
	int n;
	
	srand(pthread_self());
	
	for(int i=0; i<20; i++){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		
		n = rand();
		umidita = n;
	
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}
		sleep(1);				
	}
	
	umidita = -1;

	pthread_exit(NULL);		
}

void *thread_C_function(void *args){
	int result;
	int n;
	
	srand(pthread_self());
	
	for(int i=0; i<20; i++){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		
		n = rand();
		consumo_energetico = n;
	
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}
		sleep(1);				
	}
	
	consumo_energetico = -1;

	pthread_exit(NULL);		
}










