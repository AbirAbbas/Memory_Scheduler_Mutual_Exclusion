#include <stdio.h>
#include <ctype.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "dine.h"
#include "LinkedListAPI.h"

pthread_mutex_t * arr;

int max;
int total;

int main(int argc, char *argv[]) {
	
    total = atoi(argv[1]);
    max = atoi(argv[2]);

	pthread_t thread[total];
	int temp[total];
	
	arr = calloc(total, sizeof(pthread_mutex_t));
    
    //creates the threads to be started
    for (int i = 0; i < total; i++) {
        temp[i] = i;
        pthread_create(&(thread[i]), NULL, &philosopher, &(temp[i]));
        pthread_mutex_init(&arr[i], NULL);
    }

	//starts all threads
    for (int c = 0; c < total; c++) pthread_join(thread[c], NULL);
    
    //free's used memory
    free(arr);

    return 0;
}

void * philosopher (void * value) {
	int * id = value;
	//min value for left fork, to avoid deadlock
	int left = minValue(*id, (*id + 1) % total);
	//max value for right fork to avoid deadlock
	int right = maxValue(*id, (*id + 1) % total);
	int counter = 0;
	
	//think
	printf("Philosopher %d thinking\n", *id + 1);
	
	while (counter < max) {
		//lock
		pthread_mutex_lock(&(arr[right]));
		pthread_mutex_lock(&(arr[left]));
		//eat
		printf("Philosopher %d eating\n", *id + 1);
		sleep(1);
		//back to thinking
		printf("Philosopher %d thinking\n", *id + 1);
		pthread_mutex_unlock(&(arr[left]));
		pthread_mutex_unlock(&(arr[right]));
		
		sleep(1);
		//release
		counter++;
	}
	
	return NULL;
}

int minValue (int a, int b) {
	if (a < b) {
		return a;
	}
	else {
		return b;
	}
}

int maxValue (int a, int b) {
	if (a > b) {
		return a;
	}
	else {
		return b;
	}
}
