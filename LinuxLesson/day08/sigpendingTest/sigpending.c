#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void sig_quit(int sig){
	printf("sig_quit\n");
	return;
}
int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments\n");
//		return -1;
//	}

	//set SIGQUIT mask blocked
	sigset_t blockset;
	sigaddset(&blockset, SIGQUIT);
	sigprocmask(SIG_BLOCK, &blockset, NULL);
	sigset_t sigset;

	
	while(1){
		sigpending(&sigset);
		signal(SIGQUIT, sig_quit);
		if(sigismember(&sigset, SIGQUIT)){
			printf("SIGQUIT is pending\n");
		}
		else{
			printf("not\n");
		}
		sleep(1);
	}
	return 0;
}
