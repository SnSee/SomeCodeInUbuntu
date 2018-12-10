#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("need 2 arguments");
		return -1;
	}
	int fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		printf("open error %s:%s", argv[1], strerror(errno));
	}
	int ret = dup2(fd, 0);
	if(ret < 0){
		printf("dup2 error:%s", strerror(errno));
	}
	char buf[100];
	scanf("%s", buf);
	printf("%s\n", buf);
	return 0;
}
