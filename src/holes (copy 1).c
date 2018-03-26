#include <stdio.h>
#include <ctype.h>
#include "holes.h"
#include "LinkedListAPI.h"

int main (int argc, char ** argv) {
		
	runScheduler(argv, firstFit);
	runScheduler(argv, bestFit);
	runScheduler(argv, worstFit);
	runScheduler(argv, nextFit);
	
	return 0;
}

void runScheduler(char ** argv, void (*scheduler)(List * processList, process * memoryList)) {
	FILE * inFile = fopen(argv[1], "r");
	
	char ID[1024];
	int size = 0;
	
	List processList = initializeList(dummyPrint, dummyDelete, dummyCompare);
	
	while (fscanf(inFile, "%s %d", ID, &size) != EOF) {
		insertBack(&processList, createProcess(ID, size));
	}
	
	fclose(inFile);
	
	process * memoryList = calloc(128, sizeof(process));
	
	for (int i = 0; i < 128; i++) {
		process p;
		strcpy(p.ID, "");
		p.size = -1;
		p.time = 1000;
		p.counter = 0;
		memoryList[i] = p;
	}
	
	scheduler(&processList, memoryList);
	
	clearList(&processList);
}

void firstFit(List * processList, process * memoryList) {

	Node * n = processList->head;
	
	int position = 0;
	int endPosition = 0;
	int size;
	int time = 0;
	
	while (n != NULL) {
		process * p = n->data;
		position = findFirstEmptySpot(memoryList, endPosition, p->size);
		endPosition = position + findEmptySpace(memoryList, position);
		size = endPosition - position;
				
		if (size >= p->size) {
			
			if (p->counter >= 3) {
				removeHead(processList);
			}
			else {

				p->time = time;
				for (int i = position; i < position + p->size; i++) {
					memoryList[i] = *p;
				}
				
				//chops down to what was used!
				endPosition = 0;
				//timer used to keep track of oldest process
				
				removeHead(processList);
				
				printf("%s loaded, #processes=%d, #holes=%d, per_memusage= , cumulative_per_memusage= \n", memoryList[position].ID, numberOfProcesses(memoryList), numberOfHoles(memoryList));
				
				for (int i = 0; i < 128; i++) {
					if (strcmp(memoryList[i].ID, "")==0) {
						printf("-");
					}
					else
					printf("%s", memoryList[i].ID);
				}
				printf("\n");

			}
			
		}
		else if (checkForMoreEmpty(memoryList, position) == 1){
			int index = returnProcessIndexByID(memoryList, findOldest(memoryList));
			int deleteSize = index + memoryList[index].size;
						
			if (index == -1 || deleteSize == -1) {
				
			}
			
			process * tempProcess = calloc(1, sizeof(process));
			
			strcpy(tempProcess->ID, memoryList[index].ID);
			tempProcess->time = time;
			tempProcess->counter = memoryList[index].counter + 1;
			tempProcess->size = memoryList[index].size;
			
			insertBack(processList, tempProcess);
			
			for (int i = index; i < deleteSize; i ++) {
				process p;
				p.size = -1;
				p.time = 1000;
				p.counter = 0;
				strcpy(p.ID, "");
				memoryList[i] = p;
			}
			
			endPosition = findFirstEmptySpot(memoryList, index);
		}
		
		
		n = processList->head;
		time++;
	}
	
}

void bestFit(List * processList, process * memoryList) {
	printf("bestFit\n");
}

void worstFit(List * processList, process * memoryList) {
	printf("worstFit\n");
}

void nextFit(List * processList, process * memoryList) {
	printf("nextFit\n");
}

process * createProcess (char ID[], int size) {
	process * p = calloc(1, sizeof(process));
	
	strcpy(p->ID, ID);
	p->size = size;
	
	return p;
}

char * dummyPrint (void * toBePrinted) {
	char * temp = calloc(1024, sizeof(char));
	
	strcat(temp, ((process*)toBePrinted)->ID);
	strcat(temp, " ");
	sprintf(temp + strlen(temp), "%d", ((process*)toBePrinted)->size);
	
	return temp;
}

void dummyDelete (void * toBeDeleted) {
	
}

int dummyCompare (const void * first, const void * second) {
	if (first == second) {
		return 1;
	}
	return 0;
}

int numberOfHoles (process * memoryList) {
	
	int startHole = 0;
	int holeCount = 0;
	
	for (int i = 0; i < 128; i++) {
		if (memoryList[i].size == -1 && startHole == 0) {
			startHole = 1;
			holeCount++;
		}
		
		if (memoryList[i].size != -1 && startHole == 1) {
			startHole = 0;
		}
	}
	
	return holeCount;
}

int numberOfProcesses (process * memoryList) {
	
	char temp[1024];
	strcpy(temp, "");

	int processCount = 0;
	
	for (int i = 0; i < 128; i++) {
		if (strcmp(temp, memoryList[i].ID) != 0 && strcmp("", memoryList[i].ID) != 0) {
			strcpy(temp, memoryList[i].ID);
			processCount++;
		}
	}
	
	return processCount;
}

char * findOldest (process * memoryList) {
	
	char * temp = calloc(1024, sizeof(char));
	int max = 1000;
	
	for (int i = 0; i < 128; i ++) {
		if (memoryList[i].time < max) {
			max = memoryList[i].time;
			strcpy(temp, memoryList[i].ID);
		}
	}
	
	return temp;
}

int findFirstEmptySpot (process * memoryList, int position, int size) {
	
	//this is part of an empty spot
	
	if (position > 0 && memoryList[position - 1].size == -1) {
		for (int i = 0; i >= 0; i --) {
			if (memoryList[i].size != -1) {
				return i + 1;
			}
		}
		return 0;
	}
	
	for (int i = position; i < 128; i ++) {
		if (memoryList[i].size == -1) {
			return i;
		}
	}
	
	return 128;
}

int findEmptySpace (process * memoryList, int position) {
	int counter = 0;
	for (int i = position; i < 128; i ++) {
		if (memoryList[i].size != -1) {
			return counter;
		}
		counter++;
	}
	
	return counter;
}

int returnProcessIndexByID(process * memoryList, char * ID) {
	for (int i = 0; i < 128; i ++) {
		if (memoryList[i].size != -1 && strcmp(memoryList[i].ID, ID) == 0) {
			return i;
		}
	}
	return -1;
}

int checkForMoreEmpty (process * memoryList, int position) {
	if (findFirstEmptySpot(memoryList, position) == 128) {
		return 0;
	}
	
	return 1;
}
