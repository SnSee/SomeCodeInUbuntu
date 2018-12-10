#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char *argv[]){
	//int fd = open("/dev/tty", O_RDWR | O_NONBLOCK);
	int fd = open("/dev/tty", O_RDWR); 
	if(fd < 0){
		//printf("read failed:%s\n", strerror(errno));
		printf("read failed:%s\n", strerror(errno));
	}
	int len_r;
	int len_w;
	char buf[100];
	while(1){
		len_r = read(fd, buf, sizeof(buf));
		if(len_r >= 0){
	//		printf("you have input something\n");
			write(1, buf, strlen(buf));
			break;
		}
		else{
			printf("len_r < 0");
		}
		printf("no input\n");
		sleep(1);
	}
	//write(1, buf, strlen(buf));
	printf("you are out\n");
	return 0;
}
