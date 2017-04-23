#ifndef QUEUE_H_
#define QUEUE_H_

#include "job.h"

typedef struct Queue
{
	Job* first;
	int size;
}Queue;

Queue* CreateQueue();
void DestroyQueue(Queue* queue);
void Enqueue(Queue* queue, Job* job);
Job* Dequeue(Queue* queue);
int GetQueueSize(Queue* queue);


#endif