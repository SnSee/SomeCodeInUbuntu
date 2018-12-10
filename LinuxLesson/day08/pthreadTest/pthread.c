#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*
 *purpose: create thread and show it's identifier
 */
void *pthread_routine(void *p){
	printf("child thread %ld, id = %p\n", (long int)p, (void*)pthread_self());
	return NULL;
}
int main(int argc, char *argv[]){
	pthread_t tid;
	pthread_t tid2;
	pthread_t tid3;
	pthread_create(&tid, NULL, pthread_routine, (void *)1);
	pthread_create(&tid2, NULL, pthread_routine, (void *)2);
	pthread_create(&tid3, NULL, pthread_routine, (void *)3);
	printf("child thread 1 tid = %p\n", (void*)tid);
	printf("child thread 2 tid = %p\n", (void*)tid2);
	printf("child thread 3 tid = %p\n", (void*)tid3);

	if(!pthread_join(tid, NULL)){
		printf("join1 succeed\n");
	}
	sleep(2);
	int err;
	if(!(err = pthread_join(tid2, NULL))){
		printf("join2 succeed, errno = %d\n", err);
	}
	printf("parent thread\n");
	return 0;
}
