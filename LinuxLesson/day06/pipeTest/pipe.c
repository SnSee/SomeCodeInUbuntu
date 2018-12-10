#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int fd[2];
	if(pipe(fd) < 0){
		printf("create pipe error:%s", strerror(errno));
	}

	//write something into the pipe
	write(fd[1], "A", 1);
	char buf[1];
	//read from the pipe
	read(fd[0], buf, 1);

	printf("buf = %c\n", buf[0]);
	return 0;
}
