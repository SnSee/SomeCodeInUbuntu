#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments");
//		return -1;
//	}
	int i = execl("/bin/ls", "ls", "-l", NULL);
	printf("after execl\n");
	printf("i = %d\n", i);
	return 0;
}
