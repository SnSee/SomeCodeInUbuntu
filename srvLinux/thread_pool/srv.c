#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * function: thread pool practice
 */
//a structure represent tasks to be disposed by the thread pool
typedef struct{
	void *(*task)(void *);//point to the task
	void *arg;			  //point to the parameters
}task_queue;
//a structure established to handle this project
//including the manager-thread and the threads that dispose tasks
typedef struct thread_pool_structure{

	pid_t manager_thread; //number of the thread that dispose all the stuff
	pid_t *work_threads;  //the threads that dispose tasks
	task_queue *task_queue;//the tasks to be disposed

	pthread_mutex_t thread_lock; //the lock to synchronize work_thread
	pthread_mutex_t stru_lock;   //lock this structure
	
	pthread_cond_t nfull_queue_cond;//indicate the task queue is not full
	pthread_cond_t nempty_queue_cond;//indicate the task queue is not empty

	int alive_threads;    //the number of thread alive
	int min_threads;	  //the minimum of threads alive
	int max_threads;	  //the maximum of threads alive
	int busy_threads;	  //the number of threads working
	int increment_threads;//the increment or decrement of threads when
						  //increase or decrease threads
	
	int task_queue_head;  //the first element of the task queue
	int task_queue_foot;  //the last element of the task queue
	int task_queue_capacity;   //the maximum of tasks
	int task_queue_cur;   //the number of tasks to be disposed

	int active;           //indicate the thread pool is used or not
};
void func1(){
	
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
