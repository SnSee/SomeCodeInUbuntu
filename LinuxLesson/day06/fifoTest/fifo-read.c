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
	char buf[100];
	int len_r = read(fd, buf, sizeof(buf));
	if(len_r < 0){
		printf("read error\n");
	}
	else{
		printf("read %d bytes data into fifo\n", len_r);
		printf("%s\n", buf);
	}
	return 0;
}
