#include "part1.h"
#include <pthread.h>
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

const int MAX_NUM_FLOORS = 20;
int max_capacity = 0;
int total_floors = 0;
sem_t sem_from;
sem_t sem_to;
sem_t* semToFloor;
sem_t* semFromFloor;
int* incoming;
int* outgoing;
int* direction;
int lift_cap = 0;

void initializeP1(int numFloors, int maxNumPeople) {

	max_capacity = maxNumPeople;
	total_floors = numFloors;

	semFromFloor = malloc(sizeof(sem_t)*numFloors);
	semToFloor = malloc(sizeof(sem_t)*numFloors);
	incoming = malloc(sizeof(int)*numFloors);
	outgoing = malloc(sizeof(int)*numFloors);
	
	sem_init (&sem_from, 0, 1);
	sem_init (&sem_to, 0, 1);

	for (int i = 0; i < numFloors; i++) {
		incoming[i] = 0;
		outgoing[i] = 0;	
		sem_init (&semFromFloor[i], 0, 0);
		sem_init (&semToFloor[i], 0, 0);
	}
}

void* goingFromToP1(void *arg) {
	struct argument* temp = NULL;
	temp = (struct argument*) arg; 

	int person_id = temp->id;
	int person_from_floor = temp->from;
	int person_to_floor = temp->to;

	sem_wait(&sem_from);
	incoming[person_from_floor]++;
	sem_post(&sem_from);

	sem_wait (&semFromFloor[person_from_floor]);
	
	sem_wait(&sem_to);
	outgoing[person_to_floor]++;
	sem_post(&sem_to);

	sem_wait(&semToFloor[person_to_floor]);
	printf("%d %d %d\n", person_id, person_from_floor, person_to_floor);
}

void startP1(){
	int current_floor = 0;
	int lift_direction = 1;
	int counter = 0;
	sleep(2);
	while (1) {
		sleep(1);

		sem_wait (&sem_to);
		while (outgoing[current_floor]) {
			lift_cap--;
			outgoing[current_floor]--;
			sem_post(&semToFloor[current_floor]);
		}
		sem_post (&sem_to);
	
		sem_wait (&sem_from);
		while (incoming[current_floor] && lift_cap < max_capacity) {
			lift_cap++;
			incoming[current_floor]--;
			sem_post (&semFromFloor[current_floor]);
		}
		sem_post (&sem_from);

		if (current_floor == (total_floors - 1))
			lift_direction = 0;

		if (lift_direction == 1)
			current_floor++;

		else if (lift_direction == 0)
			current_floor--;

		if (current_floor == 0) {
			break;
		}
	}
}