#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

/*
 *function:  The  function ttyname() returns a pointer to the 
 *null-terminated pathname of the terminal device that is open 
 *on the file descriptor fd, or NULL  on  error  (for  example, 
 *if fd is not connected to a terminal).
 *
 *problem: I don't know why when the parameter is 0,1 or 2 calling 
 *ttyname(), the returned pathnames are all the same(/dev/pts/0).
 */

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("need 2 arguments\n");
		return -1;
	}
	
	int fd = open(argv[1], O_RDONLY|O_CREAT, 0644);
	char *buf0;
	char *buf1;
	char *buf_fd;
	buf0 = ttyname(0);
	if(buf0 == NULL){
		printf("invalic descriptor\n");
	}
	else
		printf("ttyname(0) returns: %s\n", buf0);

	buf1 = ttyname(1);
	buf_fd = ttyname(fd);
	if(buf1 == NULL){
		printf("invalic descriptor\n");
	}
	else
		printf("ttyname(1) returns: %s\n", buf1);
	if(buf_fd == NULL){
		printf("invalid descriptor fd\n");
	}
	else{
		printf("ttyname() returns: %s\n", buf_fd);
	}
	return 0;
}
