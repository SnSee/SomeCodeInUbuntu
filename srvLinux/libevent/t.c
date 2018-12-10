#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*
 * function: 
 */
void func1(char *filename){
	int fd = open(filename, O_WRONLY|O_NONBLOCK);
	if(fd < 0){
		printf("open failed\n");
		return;
	}	
	else{
		printf("open succeed\n");
	}
}

int main(int argc, char *argv[])
{
	func1(argv[1]);

	return 0;
}
