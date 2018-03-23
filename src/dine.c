#include <stdio.h>
#include <ctype.h>
#include "LinkedListAPI.h"

int main (int argc, char ** argv) {
	
	if (argc > 2) {
		for (int i = 0; i < strlen(argv[1]); i++) {
			if (!isdigit(argv[1][i])) {
				printf("Invalid command entered : %s\n", argv[1]);
				return 0;
			}
		}
		for (int i = 0; i < strlen(argv[2]); i++) {
			if (!isdigit(argv[2][i])) {
				printf("Invalid command entered : %s\n", argv[2]);
				return 0;
			}
		}
	}
	
	int numOfDiners = atoi(argv[1]);
	int numOfThoughts = atoi(argv[2]);
	
	int philosophers[numOfDiners];
	int left = 0;
	int right = 0;
	
	for (int i = 0; i < numOfDiners; i++) {
		philosophers[i] = i;
	}
	
	return 0;
}
