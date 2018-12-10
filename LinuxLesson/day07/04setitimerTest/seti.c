#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

//self set action responding to SIGALRM signal
void resSig(){
	printf("in sig\n");
	return;
}
int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments\n");
//		return -1;
//	}
	//catch SIGALRM with resSig()
	if(SIG_ERR == signal(SIGALRM, resSig)){
		printf("signal error\n");
	}

	struct itimerval newtimer;
	newtimer.it_interval.tv_sec = 1;
	newtimer.it_interval.tv_usec = 999*1000;
	newtimer.it_value.tv_sec = 3;
	newtimer.it_value.tv_usec = 0;
	
	setitimer(ITIMER_REAL, &newtimer, NULL);
	while(1){
		printf("alive\n");
		sleep(1);
	}
	return 0;
}
