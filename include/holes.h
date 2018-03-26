#ifndef HOLES_H
#define HOLES_H

#include "LinkedListAPI.h"

typedef struct process {
	char ID[1024];
	int size;
	int time;
	int counter;
} process;


void runScheduler(char ** argv, void (*scheduler)(List * processList, process * memoryList));

void firstFit(List * processList, process * memoryList);

void bestFit(List * processList, process * memoryList);

void worstFit(List * processList, process * memoryList);

void nextFit(List * processList, process * memoryList);

process * createProcess (char ID[], int size);

char * dummyPrint (void * toBePrinted);

void dummyDelete (void * toBeDeleted);

int dummyCompare (const void * first, const void * second);

int numberOfHoles (process * memoryList);

char * findOldest (process * memoryList);

int findFirstEmptySpot (process * memoryList, int size);

int findEmptySpace (process * memoryList, int position);

int returnProcessIndexByID(process * memoryList, char * ID);

int numberOfProcesses (process * memoryList);

int checkForMoreEmpty (process * memoryList, int position);

double checkmemUsage (process * memoryList);

int findLargestEmptySpot (process * memoryList, int size);

int findNextEmptySpot (process * memoryList, int currentPosition ,int size);

int findBestEmptySpot (process * memoryList, int size);
#endif
