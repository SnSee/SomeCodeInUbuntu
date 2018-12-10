#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[]){
	if(argc != 2){
		printf("need 2 arguments\n");
		return -1;
	}
	int fd = open(argv[1], O_RDWR);
	lseek(fd, 10, SEEK_END);
	int len_w = write(fd, "aaa", 4);
	if(len_w < 0){
		printf("write error");
	}
	return 0;
}
