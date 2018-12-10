#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

/*
 *function: implement communication in parent and child processes by mmap
 *usage: ./mmap filename
 */
int main(int argc, char *argv[]){
	if(argc != 2){
		printf("need 2 arguments\n");
		return -1;
	}
	int fd = open(argv[1], O_RDWR|O_CREAT, 0644);
	if(fd < 0){
		printf("open error\n");
	}
	//expend the file by lseek
	lseek(fd, 99, SEEK_SET);
	write(fd, " ", 1);
	//create mapping
	void *p = mmap(NULL, 100, PROT_READ|PROT_WRITE, 
			MAP_SHARED, fd, 0);
	//input data
	*(char *)p = 'p';
	pid_t pid = fork();
	if(pid < 0){
		printf("fork error\n");
	}
	if(pid == 0){
		//change first character in child process
		*(char *)p = 'c';	
		//remember to exit in each child process
		exit(0);
	}
	//print the character in parent process to check
	//whether it has been changed
	//sleep(2);
	//since sleep can't guarantee parent process executing after
	//child process, and mmap() doesn't implement this too, it 
	//would be better to control it by yourself

	//use the first character to indicate whether the child process 
	//has been executed or not
	while(1){
		if(*(char *)p == 'c'){
			printf("%c\n", *(char *)p);
			break;
		}
	}
	//printf("%c\n", *(char *)p);
	return 0;
}
