#include <stdio.h>
#include <ctype.h>
#include "holes.h"
#include "LinkedListAPI.h"

int main (int argc, char ** argv) {
		
	runScheduler(argv, firstFit);
	printf("\n\n");
	runScheduler(argv, bestFit);
	printf("\n\n");
	runScheduler(argv, worstFit);
	printf("\n\n");
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
	
	printf("First Fit Algorithm\n");

	Node * n = processList->head;
	
	int position = 0;
	int endPosition = 0;
	int size;
	int time = 0;
	
	double totalmemusage = 0;
	int totalprocesses = 0;
	int totalholes = 0;
	int totalcount = 0;
	
	while (n != NULL) {
		process * p = n->data;
		position = findFirstEmptySpot(memoryList, p->size);

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
				
				totalcount++;
				totalmemusage += checkmemUsage(memoryList);
				totalprocesses += numberOfProcesses(memoryList);
				totalholes += numberOfHoles(memoryList);
				
				
				printf("%s loaded, #processes=%d, #holes=%d, per_memusage=%lf, cumulative per_memusage=%lf\n", memoryList[position].ID, numberOfProcesses(memoryList), numberOfHoles(memoryList), checkmemUsage(memoryList), totalmemusage/(double)totalcount);
				
			}
			
		}
		else {
			
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
		}
		
		
		n = processList->head;
		time++;
	}
	
	printf("Total loads = %d, avg procs = %lf, avg holes = %lf, cumulative per_memusage=%lf\n", totalcount, ((double)totalprocesses/(double)totalcount), ((double)totalholes/(double)totalcount), totalmemusage/(double)totalcount);
	
}

void bestFit(List * processList, process * memoryList) {
	
	printf("Best Fit Algorithm\n");
	
	Node * n = processList->head;
	
	int position = 0;
	int endPosition = 0;
	int size;
	int time = 0;
	
	double totalmemusage = 0;
	int totalprocesses = 0;
	int totalholes = 0;
	int totalcount = 0;
	
	while (n != NULL) {
		process * p = n->data;
		position = findBestEmptySpot(memoryList, p->size);

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
				
				totalcount++;
				totalmemusage += checkmemUsage(memoryList);
				totalprocesses += numberOfProcesses(memoryList);
				totalholes += numberOfHoles(memoryList);
								
				printf("%s loaded, #processes=%d, #holes=%d, per_memusage=%lf, cumulative per_memusage=%lf\n", memoryList[position].ID, numberOfProcesses(memoryList), numberOfHoles(memoryList), checkmemUsage(memoryList), totalmemusage/(double)totalcount);
				
			}
			
		}
		else {
			
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
		}
		
		
		n = processList->head;
		time++;
	}
	
	printf("Total loads = %d, avg procs = %lf, avg holes = %lf, cumulative per_memusage=%lf\n", totalcount, ((double)totalprocesses/(double)totalcount), ((double)totalholes/(double)totalcount), totalmemusage/(double)totalcount);
	
}

void worstFit(List * processList, process * memoryList) {
	
	printf("Worst Fit Algorithm\n");
	
	Node * n = processList->head;
	
	int position = 0;
	int endPosition = 0;
	int size;
	int time = 0;
	
	double totalmemusage = 0;
	int totalprocesses = 0;
	int totalholes = 0;
	int totalcount = 0;
	
	while (n != NULL) {
		process * p = n->data;
		position = findLargestEmptySpot(memoryList, p->size);

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
				
				totalcount++;
				totalmemusage += checkmemUsage(memoryList);
				totalprocesses += numberOfProcesses(memoryList);
				totalholes += numberOfHoles(memoryList);
				
				
				printf("%s loaded, #processes=%d, #holes=%d, per_memusage=%lf, cumulative per_memusage=%lf\n", memoryList[position].ID, numberOfProcesses(memoryList), numberOfHoles(memoryList), checkmemUsage(memoryList), totalmemusage/(double)totalcount);
				
			}
			
		}
		else {
			
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
		}
		
		
		n = processList->head;
		time++;
	}
	
	printf("Total loads = %d, avg procs = %lf, avg holes = %lf, cumulative per_memusage=%lf\n", totalcount, ((double)totalprocesses/(double)totalcount), ((double)totalholes/(double)totalcount), totalmemusage/(double)totalcount);
	
}

void nextFit(List * processList, process * memoryList) {
	
	printf("Next Fit Algorithm\n");
	
	Node * n = processList->head;
	
	int position = 0;
	int oldPosition = 0;
	int endPosition = 0;
	int size;
	int time = 0;
	
	double totalmemusage = 0;
	int totalprocesses = 0;
	int totalholes = 0;
	int totalcount = 0;
	
	while (n != NULL) {
		process * p = n->data;
		position = findNextEmptySpot(memoryList, oldPosition, p->size);

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
				
				totalcount++;
				totalmemusage += checkmemUsage(memoryList);
				totalprocesses += numberOfProcesses(memoryList);
				totalholes += numberOfHoles(memoryList);
				
				
				printf("%s loaded, #processes=%d, #holes=%d, per_memusage=%lf, cumulative per_memusage=%lf\n", memoryList[position].ID, numberOfProcesses(memoryList), numberOfHoles(memoryList), checkmemUsage(memoryList), totalmemusage/(double)totalcount);
				
			}
			oldPosition = position;
			
		}
		else {
			
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
		}
		
		
		n = processList->head;
		time++;
	}
	
	printf("Total loads = %d, avg procs = %lf, avg holes = %lf, cumulative per_memusage=%lf\n", totalcount, ((double)totalprocesses/(double)totalcount), ((double)totalholes/(double)totalcount), totalmemusage/(double)totalcount);
	
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

int findFirstEmptySpot (process * memoryList, int size) {
	
	for (int i = 0; i < 128; i ++) {
		if (memoryList[i].size == -1 && findEmptySpace(memoryList, i) >= size) {
			return i;
		}
	}
	return 128;
}

int findNextEmptySpot (process * memoryList, int currentPosition ,int size) {
	
	//scan from now till end
	for (int i = currentPosition; i < 128; i ++) {
		if (memoryList[i].size == -1 && findEmptySpace(memoryList, i) >= size) {
			return i;
		}
	}
	
	//scan from begining till end
	for (int i = 0; i < 128; i ++) {
		if (memoryList[i].size == -1 && findEmptySpace(memoryList, i) >= size) {
			return i;
		}
	}
	
	return 128;
}

int findLargestEmptySpot (process * memoryList, int size) {
	
	int index = 0;
	int max = 0;
	
	for (int i = 0; i < 128; i ++) {
		if (i == 0 && memoryList[i].size == -1) {
			max = findEmptySpace(memoryList, i);
			index = i;
		}
		else if (memoryList[i].size == -1 && memoryList[i - 1].size != -1 && findEmptySpace(memoryList, i) > max) {
			max = findEmptySpace(memoryList, i);
			index = i;
		}
	}
	
	if (max < size) {
		return 128;
	}
	else {
		return index;
	}

}

int findBestEmptySpot (process * memoryList, int size) {
	
	int index = 0;
	int difference = 128;
	
	for (int i = 0; i < 128; i ++) {
		if (i == 0 && memoryList[i].size == -1 && findEmptySpace(memoryList, i) > size) {
			difference = findEmptySpace(memoryList, i) - size;
			index = i;
		}
		else if (memoryList[i].size == -1 && memoryList[i - 1].size != -1 && (findEmptySpace(memoryList, i) - size) < difference && (findEmptySpace(memoryList, i) - size) >= 0) {
			difference = findEmptySpace(memoryList, i) - size;
			index = i;
		}
	}
	
	//printf("index : %d\n", index);
	
	if (findEmptySpace(memoryList, index) < size) {
		return 128;
	}
	else {
		return index;
	}

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

double checkmemUsage (process * memoryList) {
	int totalusage = 0;
	
	for (int i = 0; i < 128; i ++) {
		if (memoryList[i].size != -1) {
			totalusage++;
		}
	}
	
	return ((double)totalusage/(double)128) * (double)100;
}

