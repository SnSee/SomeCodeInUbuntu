#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#define N 10

int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments");
//		return -1;
//	}
	int i;
	int pid;
	for(i = 0;i < N; i++){
		pid = fork();
		if(pid == 0)
			break;
	}
	pid = getpid();
	if(i != N){
		printf("%d sub-progress:%d\n", i, pid);
		exit(0);
	}
	else{
		printf("%d par-progress:%d\n", i, pid);
	}
	return 0;
}
