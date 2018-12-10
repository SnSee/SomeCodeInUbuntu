#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//pid_t fork(void);
int a = 10;
int b = 100;
int main(int argc, char *argv[]){
	int *p = &b;
	int pid = 10;
	pid = fork();
	if(pid == 0){
		a += 1;
		(*p) += 1;
		printf("sub:a = %d, &a = %p\n", a, &a);
		printf("sub:b = %d, &b = %p\n", b, &b);
		exit(0);
	}	
	wait(NULL);
	printf("pid = %d\tppid = %d\n", getpid(), getppid());
	printf("pid = %d\n", pid);
	a += 10;
	(*p) += 10;
	printf("par:a = %d, &a = %p\n", a, &a);
	printf("par:b = %d, &b = %p\n", b, &b);
	return 0;
}
