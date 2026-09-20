/*
 *	Scrivere un programma in C in cui il main thread genera 6 thread secondari, di cui tre del tipo inc_thread e 3 del tipo mul_thread.
 *
 *	Tutti i thread condividono una variabile intera inizializzata a 1.
 *
 *	Ogni inc_thread, ciclicamente, incrementa la variabile condivisa di 1, se e solo se il suo valore è diverso da 0.
 *	In caso contrario termina la propria esecuzione. 
 *  Ogni thread inc_thread va in sleep per 1 secondo ad ogni ciclo (chiamare la funzione sleep fuori dalla sezione critica).
 *
 *	Ogni mul_thread, ciclicamente, moltiplica la variabile condivisa per un valore casuale compreso tra 1 e 3, se e solo se
 *	il valore della stessa è diverso da 0. In caso contrario termina la propria esecuzione. 
 *  Ogni thread mul_thead va in sleep per 2 secondi ad ogni ciclo (chiamare la funzione sleep fuori dalla sezione critica).
 *
 *	Il main thread, ciclicamente, ad intervalli di 1 secondo, stampa il valore della variabile condivisa, se e solo se è pari.
 *	Dopo 12 iterazioni, il main thread assegna il valore 0 alla variabile condivisa, attende la terminazione dei thread secondari
 *	e termina esso stesso.
*/
 
// LIBRERIE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// PROTOTIPI 
void *inc_thread_function(void *args);
void *mul_thread_function(void *args);

// COSTANTI, VARIABILI GLOBALI
pthread_mutex_t mutex;
int variabile_condivisa;
int j;
#define NUM_THREAD 6
#define NUM_CYCLES 12

// MAIN
int main(void){
	variabile_condivisa = 1;
	pthread_t thread[NUM_THREAD];
	int result;
	int i;
	
	result = pthread_mutex_init(&mutex, NULL);
	if(result != 0){
		perror("pthread_mutex_init");
		exit(EXIT_FAILURE);
	}
	
	for(int i=0; i<NUM_THREAD; i++){
		if(i%2 == 0){
			result = pthread_create(&(thread[i]), NULL, inc_thread_function, NULL);
			if(result != 0){
				perror("pthread_create");
				exit(EXIT_FAILURE);
			}
		}else{
			result = pthread_create(&(thread[i]), NULL, mul_thread_function, NULL);
			if(result != 0){
				perror("pthread_create");
				exit(EXIT_FAILURE);
			}			
		}
	}
	
	for(j=0; j<NUM_CYCLES; j++){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);			
		}
		if(variabile_condivisa%2 == 0){
			printf("\n[MAIN THREAD]La Variabile Condivisa e': %d",variabile_condivisa);
		}
			
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);			
		}		
		sleep(1);
	}
	
	variabile_condivisa = 0;
	printf("\nDopo la Fine delle Iterazioni del Main Thread il valore della Variabile Condivisa viene portato a: %d",variabile_condivisa);
	
	for(i=0; i<NUM_THREAD; i++){
		result = pthread_join(mutex, NULL);
		if(result != 0){
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}
	
	result = pthread_mutex_destroy(&mutex);
	if(result != 0){
		perror("pthread_mutex_destroy");
		exit(EXIT_FAILURE);
	}
}

// FUNZIONI
void *inc_thread_function(void *args){
	int result;
	
	while(j<NUM_CYCLES){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);			
		}
		
		if(variabile_condivisa != 0){
			variabile_condivisa++;
		}
		
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);			
		}		
		sleep(1);					
	}		
	pthread_exit(NULL);
}

void *mul_thread_function(void *args){
	int result;
	int n;
	
	srand(time(NULL));
		
	while(j<NUM_CYCLES){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);			
		}
		
		if(variabile_condivisa != 0){
     	//  n = (rand()%(max-min+1))+min;
	    	n = (rand()%(3-1+1))+1;
		}
		
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);			
		}		
		sleep(2);					
	}		
	pthread_exit(NULL);
}




