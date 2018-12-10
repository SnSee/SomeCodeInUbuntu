#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

//pid_t fork(void);

int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments");
//	}
	printf("before fork\n");	
	printf("before fork2");	
	int i = 10;
	pid_t pid = fork();
	int j = 100;
	if(pid < 0){
		printf("fork failed\n");
	}
	else if(0 == pid){
		i += 10;
		j += 100;
		printf("\nchild process: ");
		printf("i = %d, j = %d\n", i, j);
	}
	else{
		i += 20;
		j += 200;
		printf("\nparent process: ");
		printf("i = %d, j = %d\n", i, j);
	}

	return 0;
}
