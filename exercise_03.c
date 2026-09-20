/*
Scrivere un programma in C in cui il main thread genera 3 thread secondari (Thread A, Thread B e Thread C).
Tutti i thread condividono tre variabili intere (VarA, VarB e VarC), inizializzate a 0.

Il main thread, ciclicamente, esegue la sottrazione [VarC-(VarA+VarB)] 
se e solo se VarC > (VarA+VarB). 

Ogni volta che il main thread esegue tale sottrazione, stampe il risultato a video e resetta le variabili condivise al valore 0. 
Il main thread ad ogni ciclo va in sleep per 2 secondi.

Il thread A, ciclicamente, assegna alla variabile VarA un valore random tra 2 e 8. 
Il thread A ad ogni ciclo va in sleep per 1 secondo.

Il thread B, ciclicamente, assegna alla variabile VarB un valore pari all’iterazione eseguita 
(es. 1 al primo ciclo, 2 al secondo ciclo, 3 al terzo, e cosi via). 
Il thread B ad ogni ciclo va in sleep per 1 secondi.

Il thread C, ciclicamente, assegna alla variabile VarC un valore random tra 5 e 15. 
Il thread C ad ogni ciclo va in sleep per 1 secondo.

Il main thread dopo 6 cicli attende la terminazione di tutti i thread e termina esso stesso.
I thread A e B dopo 10 cicli ciascuno terminano.

Utilizzare il meccanismo dei pthread_mutex per regolare l’accesso in sezione critica.
Il main thread resetta solo le variabili condivise (VarA, VarB e VarC), non i contatori dei cicli.
*/

// LIBRERIE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// PROTOTIPI
void *Thread_A_Function(void *args);
void *Thread_B_Function(void *args);
void *Thread_C_Function(void *args);

// COSTANTI, VARIABILI GLOBALI
#define NUM_CYCLES_THREAD 10
#define NUM_CYCLES_MAIN_THREAD 6
pthread_mutex_t mutex;
int VarA;
int VarB;
int VarC;	
int j;

// MAIN
int main(void){
	pthread_t thread_A;
	pthread_t thread_B;
	pthread_t thread_C;
	int result;
	int risultato;
	
	VarA = 0;
	VarB = 0;
	VarC = 0;

	result = pthread_mutex_init(&mutex, NULL);
	if(result != 0){
		perror("pthread_mutex_init");
		exit(EXIT_FAILURE);
	}
	
	result = pthread_create(&(thread_A),NULL,Thread_A_Function,NULL);
	if(result != 0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	result = pthread_create(&(thread_B),NULL,Thread_B_Function,NULL);
	if(result != 0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}	
	
	result = pthread_create(&(thread_B),NULL,Thread_C_Function,NULL);
	if(result != 0){
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	
	for(j = 0; j < NUM_CYCLES_MAIN_THREAD; j++){
		result = pthread_mutex_lock(&mutex);
		if (result != 0){
			perror("pthread_lock");
			exit(EXIT_FAILURE);		
		}
		if(VarC > (VarA + VarB)){
			
			risultato = VarC - (VarA + VarB);
			printf("\n[MAIN THREAD]: La Sottrazione ha il seguente Valore: %d",risultato);
			
			VarA = 0;
			VarB = 0;
			VarC = 0;		
		}
		result = pthread_mutex_unlock(&mutex);		
		if (result != 0){
			perror("pthread_unlock");
			exit(EXIT_FAILURE);			
		}	
		sleep(2);
	}
	
	result = pthread_join(thread_A, NULL);
	if(result != 0){
		perror("pthread_joinA");
		exit(EXIT_FAILURE);
	}
	
	result = pthread_join(thread_B, NULL);
	if(result != 0){
		perror("pthread_joinB");
		exit(EXIT_FAILURE);
	}	

	result = pthread_join(thread_C, NULL);
	if(result != 0){
		perror("pthread_joinC");
		exit(EXIT_FAILURE);
	}		
	
	pthread_mutex_destroy(&mutex);
	if(result != 0){
		perror("pthread_mutex_destroy");
		exit(EXIT_FAILURE);
	}
}

// FUNZIONI
void *Thread_A_Function(void *args){
	int result;
	int n;
	
	srand(time(NULL));
	
	for(int i=0; i<NUM_CYCLES_THREAD; i++){
		result = pthread_mutex_lock(&mutex);
		if (result != 0){
			perror("pthread_lock");
			exit(EXIT_FAILURE);		
		}
		
		//  n = (rand()%(max-min+1))+min;
		n = (rand()%(8-2+1)+2);
		VarA = n;
		printf("\nVarA = %d",VarA);	
			
		result = pthread_mutex_unlock(&mutex);
		if (result != 0){
			perror("pthread_unlock");
			exit(EXIT_FAILURE);		
		}	
		sleep(2);
	}
	pthread_exit(NULL);
}

void *Thread_B_Function(void *args){
	int result;	
	
	for(int i=0; i<NUM_CYCLES_THREAD; i++){
		result = pthread_mutex_lock(&mutex);
		if (result != 0){
			perror("pthread_lock");
			exit(EXIT_FAILURE);		
		}
		
		VarB++;
		printf("\nVarB = %d",VarB);
				
		result = pthread_mutex_unlock(&mutex);
		if (result != 0){
			perror("pthread_unlock");
			exit(EXIT_FAILURE);		
		}
		sleep(1);			
	}
	pthread_exit(NULL);
}

void *Thread_C_Function(void *args){
	int result;
	int n;
	
	srand(time(NULL));
	
	while(j<NUM_CYCLES_MAIN_THREAD){	
		result = pthread_mutex_lock(&mutex);
		if (result != 0){
			perror("pthread_lock");
			exit(EXIT_FAILURE);		
		}
	
		//  n = (rand()%(max-min+1))+min;	
		n = (rand()%(15-5+1)+5);
		VarC = n;
		printf("\nVarC = %d",VarC);
	
		result = pthread_mutex_unlock(&mutex);
		if (result != 0){
			perror("pthread_unlock");
			exit(EXIT_FAILURE);		
		}	
		sleep(1);
	}
	pthread_exit(NULL);
}

