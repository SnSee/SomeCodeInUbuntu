#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){

	int fd;
	int fd2;
	fd = open("hello.c", O_RDONLY);
	fd2 = open("hello.c",O_RDONLY);
	if(fd<0){
		perror("open hello.c");
		return -1;
	}
	if(fd2<0){
		perror("fd2 open hello.c");
		return -2;
	}
	printf("fd = %d\n", fd);
	printf("fd2 = %d\n", fd2);
	close(fd);	
	close(fd2);	
}
