#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	int pid = fork();
	if(pid == 0){
		sleep(1);
		printf("child process\n");
		sleep(3);
		exit(0);
	}

	int wstatus;
	pid_t p = wait(&wstatus);
	if(WIFEXITED(wstatus)){
		printf("child process %d exited\n", pid);
	}
	else{
		printf("child precess stopped\n");
	}
	return 0;
}
