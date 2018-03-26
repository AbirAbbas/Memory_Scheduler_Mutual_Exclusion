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
	int left = minValue(*id, (*id + 1) % total);
	int right = maxValue(*id, (*id + 1) % total);
	int counter = 0;
	while (counter < max) {
		//think
		printf("Philosopher %d thinking\n", *id + 1);
		//lock
		pthread_mutex_lock(&(arr[right]));
		pthread_mutex_lock(&(arr[left]));
		//eat
		printf("Philosopher %d eating\n", *id + 1);
		sleep(1);
		pthread_mutex_unlock(&(arr[left]));
		pthread_mutex_unlock(&(arr[right]));
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
