#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char *argv[]){
	if(argc < 2){
		printf("too few arguments");
	}
	int fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		printf("open failed:%s",strerror(errno));
	}

	char buf[1000];
	char ch[10];
	int len_r;
	len_r = read(fd,buf,sizeof(buf));
	int lch = read(fd,ch,sizeof(ch));
	printf("len_r = %d\n", len_r);
	printf("lch = %d\n",lch);
	if(len_r < 0){
		printf("read failed:%s", strerror(errno));
	}
	int len_w;
	len_w = write(1,buf,len_r);
	if(len_w != len_r){
		printf("write error");
	}
	close(fd);
	return 0;
}
