#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
	close(0);
	int fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		printf("open error:%s", strerror(errno));
	}
	dup(fd);
	char buf[100];
	scanf("%s", buf);
	printf("%s\n", buf);
	return 0;
}
