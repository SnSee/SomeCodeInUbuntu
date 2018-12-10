#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments\n");
//		return -1;
//	}
	pid_t pid = fork();
	if(pid == 0){
		while(1){
			printf("still alive\n");
			sleep(1);
		}
		exit(0);
	}
	sleep(5);
	int ret = kill(pid, SIGKILL);
	if(!ret){
		printf("no, you aren't\n");
	}
	else{
		printf("kill error\n");
	}
	//remember to release the resourses associated with the child process
	return 0;
}
