#include <stdio.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_PERMS 520
#define PERMUTATION_SIZE 10
char *alphabet="abcdefghijklmnopqrstuvwxyz";
int i=0;
int j=0;
char permutations[PERMUTATION_SIZE];
sem_t full , empty, mutex;
bool hasMore=true;



void *producer()
{
    do
    {
        char item=alphabet[i];
        sem_wait(&empty);
	sem_wait(&mutex);
	printf("Enter Producer Critical Section...\n");
        /* 
	 * Critical section of producer
	 */
	
	printf("Producing %c.\n", item);
        permutations[i]=item;
	i=(i+1)%10;
	j=j+1;
	sem_post(&mutex);
	sem_post(&full);
	printf("Exit Producer Critical Section ... \n");
    } while(j<520);
    hasMore=false;
    pthread_exit(0);
}


void *consumer()
{
    do
    {
        char consumedCharacters[10];
        int g=0;
        sem_wait(&full);
        sem_wait(&mutex);
	printf("Enter consumer critical section...\n");
        do
        {
            if(permutations[g] == ' ')
	    {
                fprintf(stderr, "There was nothing to consume.");
            }
	    else
            {
	        printf("Consuming %c.\n", permutations[g]);
                consumedCharacters[g]=permutations[g];
	    }
            g=g+1; 
        }while(g < 10);
        sem_post(&mutex);
        sem_post(&empty);
	printf("Exit consumer critical section...\n");
        printf("Recieved permutation from the producer %s \n", \
		        consumedCharacters);
    }while(hasMore);
    pthread_exit(0);
}

void main()
{
    pthread_t tidOne, tidTwo;
    pthread_attr_t attr;
    printf("Initializing semaphores\n");
    sem_init(&empty, 0, PERMUTATION_SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);
    //initializes thread to default attributes
    pthread_attr_init(&attr);
    //creating producer thread
    printf("Initializing producer\n");
    pthread_create(&tidOne, &attr, producer, NULL);
    printf("Initializing consumer\n");
    pthread_create(&tidTwo, &attr, consumer, NULL);
    printf("Joining producer\n");
    pthread_join(tidOne, NULL);
    printf("Joining consumer\n");
    pthread_join(tidTwo, NULL);
}

