#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("need 2 arguments\n");
		return -1;
	}
	int fd = open(argv[1], O_RDWR);
	if(fd < 0){
		printf("open error");
	}
	char buf[] = "communicate with other process";
	int len_w = write(fd, buf, strlen(buf));
	if(len_w < 0){
		printf("write error\n");
	}
	else{
		printf("write %d bytes data into fifo\n", len_w);
	}
	return 0;
}
