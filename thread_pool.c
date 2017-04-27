#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "thread_pool.h"

ThreadPool* CreateThreadPool(int num){
  if(num<1){
    //printf("Invalid input: number of worker\n");
    return NULL;
  }
  ThreadPool* newtp=(ThreadPool*)malloc(sizeof(ThreadPool));
  newtp->numWorker = num;
  newtp->ifactive = FALSE;
  newtp->queue =CreateQueue();
  newtp->workers = (pthread_t*)malloc(num*sizeof(pthread_t));
  newtp->joinable = (pthread_attr_t*)malloc(num*sizeof(pthread_attr_t));
  return newtp;
}

void DestroyThreadPool(ThreadPool* pool){
  if(!pool){
    //printf("Invalid input: Pointer of pool\n");
    return;
  }
  
  DestroyQueue(pool->queue);
  free(pool->workers);
  free(pool->joinable);
  free(pool);
  pool->workers = NULL;
  pool->queue=NULL;
}

void EnqueueJob(ThreadPool* pool, Job* jobs){
  if(!pool){
    printf("Invalid input: Pointer of pool\n");
    return;
  }
    if(!jobs){
    printf("Invalid input: Pointer of Job\n");
    return;
  }
    Enqueue(pool->queue,jobs);
}
void * execute (void* argument){
  if(!argument){
    return NULL;
  }
  Queue * arg = (Queue*)argument;
  Job* job ;
  pthread_mutex_lock(arg->mutex);
  job=Dequeue(arg);
  (*job->job_description)(job->input,job->output);
  //printf("%d\n",GetQueueSize(arg));
  pthread_mutex_unlock(arg->mutex);
}

void  ExecuteJobs(ThreadPool* pool){
  //printf("%d\n",GetNumberOfRemainingJobs(pool));
  if(!pool){
    //printf("Invalid input: Pointer of pool\n");
    return ;
  }
  //printf("%d===%d\n",GetQueueSize(pool->queue),pool->numWorker);
  /*
  if(GetQueueSize(pool->queue)!=pool->numWorker){
    //printf("wrong\n");
    return;
    
  }*/
  pthread_t * worker= pool->workers;
  pthread_mutex_t mutex ;
  pthread_attr_setdetachstate(pool->joinable,PTHREAD_CREATE_JOINABLE);
  pthread_mutex_init(&mutex,NULL);
  pool->queue->mutex = &mutex;
  Queue * q = pool->queue;
  int size =GetQueueSize(pool->queue);
  while(GetQueueSize(pool->queue)){
    for(int n = 0 ; n < pool->numWorker&&GetQueueSize(pool->queue);n++){
      int i = n%(pool->numWorker);
      if(pthread_create(&worker[n],
			pool->joinable,
			*execute,
			q)){
	printf("failed\n");
      }
      pthread_join(worker[n],NULL);
    }
  }
  //printf("1111\n");
  pool->ifactive = TRUE;
  pthread_attr_destroy(pool->joinable);
  pthread_mutex_destroy(&mutex);
  //  pthread_exit(NULL);
}

int IsThreadPoolActive(ThreadPool* pool){

  if(!pool){
    //printf("Invalid input: Pointer of pool\n");
    return -1;
  }
  return pool->ifactive;
}

int GetNumberOfRemainingJobs(ThreadPool* pool){
  if(!pool){
    //printf("Invalid input: Pointer of pool\n");
    return -1;
  } 
  return GetQueueSize(pool->queue);
}
