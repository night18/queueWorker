#ifndef thread_pool_h_
#define thread_pool_h_

#include <pthread.h>
#include "queue.h"

#define TRUE 1
#define FALSE 0

typedef struct ThreadPool{
  int numWorker;
  int ifactive;
  Queue * queue;
  pthread_t * workers;
  pthread_attr_t * joinable;
}ThreadPool;

//constructor
ThreadPool* CreateThreadPool(int num);

//destructor
void DestroyThreadPool(ThreadPool* pool);

//EnqueueJob
void EnqueueJob(ThreadPool* pool,Job * jobs);

//execute jobs
void ExecuteJobs(ThreadPool* pool);

//is thread pool active
int IsThreadPoolActive(ThreadPool* pool);

//get number of remaining jobs
int GetNumberOfRemainingJobs(ThreadPool* pool);

#endif
