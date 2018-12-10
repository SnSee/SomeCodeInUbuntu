#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("need 2 arguments\n");
		return -1;
	}
	int fd = open(argv[1], O_RDWR|O_CREAT, 0644);
	if(fd < 0){
		printf("read error\n");
	}
	char buf[100];
	read(fd, buf, sizeof(buf));
	printf("buf = %s\n", buf);
	close(fd);
	return 0;
}
