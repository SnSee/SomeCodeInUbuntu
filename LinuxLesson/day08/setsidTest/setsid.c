#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>

/*
 *function: create a daemon by calling setsid() in a child process
 *
 */

int main(int argc, char *argv[]){
	//1.create a child process
	pid_t pid;
	if((pid = fork()) < 0){
		perror("fork ");
	}
	if(pid != 0){
		exit(0);
	}

	//in child process
	if( 0 > setsid()){
		perror("setsid ");
		exit(1);
	}	
	close(0);
	//the open file descriptor would be 0 since 0 is unused now
	open("/dev/null", O_RDWR);
	dup2(0, 1);
	dup2(1, 2);

	//change working directory
	if(chdir("/") < 0){
		perror("chdir ");
		exit(1);
	}

	//keep the process alive
	while(1){
	}
	return 0;
}
