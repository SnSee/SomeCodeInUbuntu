#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

void sig_handler(){
	printf("in sig_handler\n");
	return;
}

int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments\n");
//		return -1;
//	}
	//initialize struct sigaction
	struct sigaction siga;
	//memset(&siga, 0, sizeof(siga));
	printf("1 sa_sigaction = %p\n", siga.sa_sigaction);
	
	siga.sa_sigaction = NULL;
	printf("2 sa_sigaction = %p\n", siga.sa_sigaction);
	siga.sa_handler = sig_handler;
	//don't assign both sa_handler and sa_sigaction
	//siga.sa_sigaction = NULL;
	sigemptyset(&siga.sa_mask);
	siga.sa_flags = 0;

	printf("3 sa_sigaction = %p\n", siga.sa_sigaction);
	printf("*sa_sigaction = %s\n", (char *)siga.sa_sigaction);
	printf("*sa_handler = %s\n", (char *)siga.sa_handler);
	if(0 > sigaction(SIGQUIT, &siga, NULL)){
		printf("sigaction error\n");
	}
	while(1){
		printf("not\n");
		sleep(1);
	}
	return 0;
}
