/*
Scrivere un programma in C in cui un main thread (M) genera 3 thread secondari.
Tutti i thread condividono una sola variabile intera, inizializzata a 0.

Ogni thread secondario, ciclicamente, accede alla variabile condivisa.
Se il valore di tale variabile è cambiato dall’ultimo accesso, il thread incrementa la variabile condivisa di un’unità e incrementa anche il numero di volte che l’ha modificata.
Indipendentemente dal fatto che l’operazione di incremento sia stata svolta o meno, l’iterazione termina dopo 10 cicli. 
Ogni thread termina con stato di terminazione pari al numero di volte che ha modificato la variabile.
In questo semplice gioco vince il thread che ha modificato più volte la variabile condivisa.

Il main thread, dopo aver creato i thread secondari, stampa a video il valore della variabile ogni volta che la trova cambiata.

Tutti i thread (compreso il main thread) ad ogni iterazione vanno in sleep per un secondo (chiamare la funzione sleep fuori dalla sezione critica).
Dopo 10 cicli il main thread attende la terminazione dei thread secondari, dichiara il vincitore e termina esso stesso.

Utilizzare il meccanismo dei pthread_mutex per regolare l’accesso alla sezione critica.
*/

// LIBRERIE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>

// PROTOTIPI
void *increment_func(void *args);

// COSTANTI, VARIABILI GLOBALI
#define NUM_THREAD 3
#define NUM_CYCLES 10
int variabile_condivisa;
pthread_mutex_t mutex;
int j;

// MAIN 
int main(void){
	int vecchio_valore;
	variabile_condivisa = 0;
	int result;	
	pthread_t thread[NUM_THREAD];
		
	result = pthread_mutex_init(&mutex, NULL);
	if(result != 0){
		perror("pthread_mutex_init");
		exit(EXIT_FAILURE);	
	}

	for(int i=0; i<NUM_THREAD; i++){
		result = pthread_create(&(thread[i]),NULL,increment_func,NULL);
		if(result != 0){
			perror("pthread_create");
			exit(EXIT_FAILURE);			
		}
	}
	
	vecchio_valore = variabile_condivisa;
	
	for(j=0; j<NUM_CYCLES; j++){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);		
		}	
		if(vecchio_valore != variabile_condivisa){
			printf("\nIl Nuovo Valore della Variabile Condivisa e': %d",variabile_condivisa);
			vecchio_valore = variabile_condivisa;
		}
		result = pthread_mutex_unlock(&mutex);		
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);		
		}
		sleep(1);			
	}
	
	void *ret_val[NUM_THREAD];
	
	for(int i=0; i<NUM_THREAD; i++){
		result = pthread_join((thread[i]),&ret_val[i]);
		if(result != 0){
			perror("pthread_join");
			exit(EXIT_FAILURE);			
		}		
		printf("\nIl Valore Restituito dal Thread %d e': %d", i+1, *((int *)ret_val[i]));
	}	
}

// FUNZIONI
void *increment_func(void *args){
	int result;
    int vecchio_valore = -1;
    
    int *modificato;
    modificato=(int*)malloc(sizeof(int)); //usiamo una variabile allocata dinamicamente per restituire un valore
    *modificato = 0;
    
	while(j<NUM_CYCLES){
		result = pthread_mutex_lock(&mutex);
		if(result != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);		
		}
		if(vecchio_valore != variabile_condivisa){
			variabile_condivisa++;
			vecchio_valore = variabile_condivisa;
			(*modificato)++;
			
		}
		result = pthread_mutex_unlock(&mutex);
		if(result != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);		
		}		
		sleep(1);	
	}
	pthread_exit((void *)modificato);
}

