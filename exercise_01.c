/*
Scrivere un programma in C in cui un processo principale (P) genera 4 thread, di cui 2 del tipo increment_thread e 2 del tipo decrement_thread.
Tutti i thread condividono una sola variabile intera, inizializzata a 0.
Ogni increment_thread, ciclicamente, 
	- incrementa di un’unità la variabile condivisa se e solo se il valore della stessa è inferiore a 10.

Ogni decrement_thread, ciclicamente, 
	- decrementa di un’unità la variabile condivisa se e solo se il valore della stessa è superiore a 5.

L’iterazione termina dopo 7 cicli, indipendentemente dal fatto che l’operazione di incremento (o decremento) sia stata svolta o meno in ognuno di essi. 
Solo quando la variabile condivisa viene modificata, un thread stampa a video:
-	L’operazione eseguita.
-	Il valore della variabile condivisa, prima e dopo l’operazione.

Tutti i thread ad ogni iterazione vanno in sleep per un secondo (chiamare la funzione sleep fuori dalla sezione critica).

Il processo principale dopo aver creato i thread attende la terminazione di tutti.
Utilizzare il meccanismo dei pthread_mutex per regolare l’accesso in sezione critica.
*/

// LIBRERIE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// PROTOTIPI
void *increment_thread(void *args);
void *decrement_thread(void *args);

// COSTANTI, VARIABILI GLOBALI
pthread_mutex_t mutex;
int variabile_condivisa;
int i;
#define NUM_THREAD 4
#define NUM_CYCLES 7

// MAIN
int main(void){
	int risultato;
	variabile_condivisa = 0;
	pthread_t thread[NUM_THREAD];
	
	risultato = pthread_mutex_init(&mutex, NULL);
	if(risultato != 0){
		perror("pthread_mutex_init");
		exit(EXIT_FAILURE);	
	}
	
	for(i=0; i<NUM_THREAD; i++){
		if(i%2 == 0){
			risultato = pthread_create(&(thread[i]),NULL,increment_thread,NULL);
			if(risultato != 0){
				perror("pthread_create");
				exit(EXIT_FAILURE);
			}
		}else{
			risultato = pthread_create(&(thread[i]),NULL,decrement_thread,NULL);
			if(risultato != 0){
				perror("pthread_create");
				exit(EXIT_FAILURE);
			}			
		}
	}
	
	for(int i=0; i<NUM_THREAD; i++){
		risultato = pthread_join(thread[i],NULL);
		if(risultato != 0){
			perror("pthread_join");
			exit(EXIT_FAILURE);
		}
	}
	
	risultato = pthread_mutex_destroy(&mutex);
	if(risultato != 0){
		perror("pthread_mutex_destroy");
		exit(EXIT_FAILURE);
	}
	
}

// FUNZIONI
void *increment_thread(void *args){
	int vecchio_valore;
	int risultato;
	for(int i=0; i<NUM_CYCLES; i++){
		risultato = pthread_mutex_lock(&mutex);
		if(risultato != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		if(variabile_condivisa < 10){
			variabile_condivisa++;
			printf("\n[INCREMENTO] - Vecchio Valore: %d, Nuovo Valore: %d", variabile_condivisa, variabile_condivisa+1);
			vecchio_valore = variabile_condivisa;
		}
		risultato = pthread_mutex_unlock(&mutex);
		if(risultato != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}	
		sleep(1);	
	}
	pthread_exit(NULL);
}

void *decrement_thread(void *args){
	int risultato;
	for(int i=0; i<NUM_CYCLES; i++){
		risultato = pthread_mutex_lock(&mutex);
		if(risultato != 0){
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}
		if(variabile_condivisa > 5){
			variabile_condivisa--;
			printf("\n[DECREMENTO] - Vecchio Valore: %d, Nuovo Valore: %d", variabile_condivisa, variabile_condivisa-1);
		}
		risultato = pthread_mutex_unlock(&mutex);
		if(risultato != 0){
			perror("pthread_mutex_unlock");
			exit(EXIT_FAILURE);
		}	
		sleep(1);	
	}
	pthread_exit(NULL);
}


