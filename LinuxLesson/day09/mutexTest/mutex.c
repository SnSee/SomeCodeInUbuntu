#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#define N 2
#define NLOOP 5000

/*
 *function: test what the result will be if program works with 
 *mutex(two threads modify the same global variable)
 */

int a = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *thread1(void *p){
	for(int i = 0; i < NLOOP; i++){
		//lock
		pthread_mutex_lock(&mutex);
		int count1 = a;
		usleep(1000);
		count1++;
		a = count1;
		//release the mutex
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
void *thread2(void *p){
	int count2 = a;
	count2++;
	a = count2;
	return NULL;
}
int main(int argc, char *argv[]){
	pthread_t tid[N];
	for(int i = 0; i < N; i++){
		//create threads
		pthread_create(&tid[i], NULL, thread1, (void *)1);
	}
	//release the resources
	for(int i = 0; i < N; i++){
		pthread_join(tid[i], NULL);
	}
	printf("a = %d\n", a);
	return 0;
}
