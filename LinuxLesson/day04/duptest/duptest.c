#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]){
	int fd;
	if(argc != 2){
		printf("need 2 arguments");
	}
	fd = open(argv[1], O_RDONLY);
	close(0);
	int newfd;
	newfd = dup2(fd,0);
	if(0 != newfd){
		printf("dup failed");
		return -1;
	}
	char buf[100] = {""};
	scanf("%s",buf);
	printf("%s",buf);
	close(fd);
	return 0;
}
