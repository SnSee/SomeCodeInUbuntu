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
	//to check if mmap change the data
	write(fd, "abc", 4);
	int length = lseek(fd, 0, SEEK_END);
	void *map = 
		mmap(NULL, length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	int i;
	scanf("%d", &i);
	//modify the first character of the file
	*(char *)map = 'A';
	char buf[100];
	read(fd, buf, sizeof(buf));
	printf("buf = %s\n", buf);
	//munmap
	munmap(map, length);
	close(fd);
	return 0;
}
