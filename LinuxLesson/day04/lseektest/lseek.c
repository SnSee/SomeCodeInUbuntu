#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){


	if(argc != 2){
		printf("need 2 arguments");
	}
	int fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		printf("open %s failed:%s", argv[0], strerror(errno));
	}
	char ch[1];
	while(1){
		int len_r = read(fd,ch,1);
		if(len_r == 0)
			break;
		int len_w = write(1,ch,1);
		if(len_w < 0)
			printf("write failed");
		lseek(fd,1,SEEK_CUR);
	}
	close(fd);
	return 0;
}

