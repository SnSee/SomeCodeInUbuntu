#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define N 10
#define NLOOP 1000

/*
 *function: test what the result will be if program works with 
 *rwlock(two threads modify the same global variable)
 */

int a = 0;
pthread_rwlock_t rwlock;
//write thread
void *thread1(void *p){
	for(int i = 0; i < NLOOP; i++){
		//write lock
		pthread_rwlock_wrlock(&rwlock);
		int count1 = a;
		//usleep(1000);
		count1++;
		a = count1;
		//release the mutex
		pthread_rwlock_unlock(&rwlock);
	}
	return NULL;
}
//read thread
void *thread2(void *p){
	for(int i = 0; i < NLOOP; i++){
		pthread_rwlock_rdlock(&rwlock);
		int count2 = a;
		pthread_rwlock_unlock(&rwlock);
	}
	return NULL;
}
int main(int argc, char *argv[]){
	//initialize the rwlock
	pthread_rwlock_init(&rwlock, NULL);
	pthread_t tid[N];
	int i = 0;
	for(; i < 3; i++){
		//create threads to write
		pthread_create(&tid[i], NULL, thread1, (void *)i);
	}
	for(; i < N; i++){
		pthread_create(&tid[i], NULL, thread2, (void *)i);
	}
	//release the resources
	for(int i = 0; i < N; i++){
		pthread_join(tid[i], NULL);
	}
	printf("a = %d\n", a);
	return 0;
}
