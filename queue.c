#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

Queue* CreateQueue(){
	Queue* queue = (Queue*) malloc(sizeof(Queue));
	if(!queue){
		return NULL;
	}
	queue -> first = NULL;
	queue -> size = 0;
}

void DestroyQueue(Queue* queue){
	if(queue){
		queue -> first = NULL;
		queue -> size = 0;
		free(queue);
		queue = NULL;
	}
}

void Enqueue(Queue* queue, Job* job){
	if(!queue){
		return;
	}
	if(!job){
		return;
	}

	if(!(queue->first)){
		queue -> first = job;
	}else{
		Job* last = (queue -> first);
		while(last->next){
			last = last -> next;
		}
		last->next = job;
	}
	(queue -> size)++;
}

Job* Dequeue(Queue* queue){
	if(!queue){
		return NULL;
	}

	if(!(queue->first)){
		return NULL;
	}

	Job* pop = queue -> first;
	//TODO
	Job* next =  queue -> first -> next;
	if(!next){
		queue -> first = NULL;
		queue -> size = 0;
	}else{
		queue -> first = next;
		(queue -> size)--;
	}

	return pop;
}

int GetQueueSize(Queue* queue){
	if(!queue){
		return -1;
	}
	return queue->size ;
}