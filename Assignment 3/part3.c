#include <pthread.h>
#include "main.h"
#include "part3.h"
#include <stdio.h>
#include <sys/queue.h>
#include <semaphore.h>

sem_t sem1;
sem_t sem2;
sem_t sem3;

TAILQ_HEAD(head_s, NODE) head[4][2];

int ALight;
int AllCars = 0;

struct NODE
{
	int c;
	int dest;
	TAILQ_ENTRY(NODE) NODEs;
};

char* getLane(int val) 
{
	char *lanes[] = {"LEFT", "RIGHT"};
	return lanes[val];
}

char* getDirection(int val) 
{
	char *directions[] = {"NORTH", "SOUTH", "EAST", "WEST"};
	return directions[val];    
}

static void InsertInsideQueue(TAILQ_HEAD(head_s, NODE)* head, int string, int dst)
{
	int c = 0;
	struct NODE *temp = malloc(sizeof(struct NODE));
	temp->c = string;
	temp->dest = dst;
	TAILQ_INSERT_TAIL(head, temp, NODEs);
	temp = NULL;
}

void dequeue(int i, int j)
{
	struct NODE * temp = TAILQ_FIRST(&head[i][j]);
	char* to = getDirection(temp->dest);
	char* l = getLane(j);
	char* from = getDirection(i);
	printf("%s %s %s \n", from, to, l);
	TAILQ_REMOVE(&head[i][j], temp , NODEs);
}

void initializeP3() 
{
	sem_init(&sem3, 0, 1);
	int i, j;
	for(i = 0; i<4; i++)
	{
		for(j=0; j<2; j++)
		{
			TAILQ_INIT(&head[i][j]);
		}
	}	
}

void popOut(){
	int number = 0;
	while(1)
	{
		number = 0;
		sem_wait(&sem1);
		while(number<5)
		{
			if(TAILQ_EMPTY(&head[ALight][0]))
			{
				break;
			}
			else
			{
				dequeue(ALight, 0);
				AllCars--;
				number++;
			}
		}
		while(number<5)
		{
			if(TAILQ_EMPTY(&head[ALight][1]))
			{
				break;
			}
			else
			{
				dequeue(ALight, 1);
				AllCars--;
				number++;
				
			}
		}
		sem_post(&sem2);
	}
}

void Print(){
	int i, j;
	i = 0;
	j = 0;
	while (i < 4)
	{
		while(j < 2)
		{
			printf("i: %d, j: %d ", i, j);
			
			struct NODE* e = NULL;
			TAILQ_FOREACH(e, &head[i][j], NODEs)
		    {
		    	printf("%d", e->c);
		    }
		    printf("\n");
			j++;
		}
		i++;
	}
}

void* TrafficFunction(){
	ALight = 0;

	while(1)
	{
		if(AllCars == 0)
		{
			return;
		}

		sem_post(&sem1);
		
		sem_wait(&sem2);

		switch(ALight){
			case 0:
			{
				ALight = 2;
				break;
			}
			case 1:
			{
				ALight = 3;
				break;
			}
			case 2:
			{
				ALight = 1;
				break;
			}
			case 3:
			{
				ALight = 0;
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

void startP3()
{
	sleep(1);
	pthread_t tid1;
	pthread_create(&tid1, NULL, popOut, NULL);
	TrafficFunction();
}

void* goingFromToP3(void *argu)
{
	struct argumentP3 *arg = (struct argumentP3*)(argu);
	sem_wait(&sem3);
	InsertInsideQueue(&head[arg->from][arg->lane], arg->user_id, arg->to);
	AllCars++;
	sem_post(&sem3);
}

