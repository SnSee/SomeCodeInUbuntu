#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments\n");
//		return -1;
//	}
	//create a sigset_t variable
	sigset_t sb;
	sigemptyset(&sb);
	sigaddset(&sb, SIGINT);
	
	sigprocmask(SIG_BLOCK, &sb, NULL);
	while(1){
		printf("alive\n");
		sleep(1);
	}
	return 0;
}
