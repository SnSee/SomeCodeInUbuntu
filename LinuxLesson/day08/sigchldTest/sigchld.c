#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
 *function: dispose SIGCHLD signal 
 *release the underlying resources
 */
void sig_chld(int sig){
	printf("the state of child process has changed\n");	
	while(waitpid(-1, NULL, WNOHANG) > 0){
		printf("waitpid succeeded\n");
	}
}
int main(int argc, char *argv[]){

	signal(SIGCHLD, sig_chld);
	pid_t pid = fork();
	if(pid == 0){
		sleep(1);
		printf("child process\n");
		exit(0);
	}

	//sleep() will return immediately when it's interrupted by
	//signal handler, in this case, it will return number of seconds
	//left to sleep
	int i =	sleep(5);
	printf("i = %d\n", i);
	printf("parent process exit\n");
	sleep(2);
	return 0;
}
