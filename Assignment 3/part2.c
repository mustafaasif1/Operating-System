#include <pthread.h>
#include "main.h"
#include "part2.h"
#include <stdio.h>
#include <semaphore.h>

const int INTER_ARRIVAL_TIME = 5;
const int NUM_TRAINS = 5;

int LockCurrentValue[5];
int total_stations;
int max_people;

sem_t* sem_people;
sem_t sem_lock;

void* TrainFunction(void* temp){
	int begin = (int)temp;
	LockCurrentValue[begin] = begin;
	while(1){
		sem_wait (&sem_lock);
		for(int i = 0; i < max_people; i++) {
			sem_post(&sem_people[i]);
		}
		sem_post (&sem_lock);
		sleep(1);
		LockCurrentValue[begin] = (LockCurrentValue[begin] + 1) % total_stations;
	}
}

void initializeP2(int numStations, int maxNumPeople) {
	total_stations = numStations;
	max_people = maxNumPeople;
	sem_people = malloc(sizeof(sem_t)*maxNumPeople);
	sem_init (&sem_lock, 0, 1);

	pthread_t trains[5];
	
}

void *goingFromToP2(void *argu) {
	int inside = 0;
	int train;
	struct argument* temp = NULL;
	temp = (struct argument*) argu; 

	int person_id = temp->id;
	int person_from = temp->from;
	int person_to = temp->to;

	while(1){
		for(int i = 0; i < 5; i++){
			sem_wait(&sem_people[person_id]);
		}
		for(int i = 0; i < 5; i++){
			if(LockCurrentValue[i] == person_from && inside!=1){
				inside = 1;
				train = i;
				break;
			}
		}
		if(LockCurrentValue[train] == person_to && inside == 1){
			printf("%d %d %d\n", train, person_from, person_to);
			pthread_exit(NULL);
		}
	}
}

void *startP2(){
	for(int i = 0; i < 5; i++) {
		pthread_t tid1;
		pthread_create(&tid1, NULL, TrainFunction, (void*)i);
	}
}