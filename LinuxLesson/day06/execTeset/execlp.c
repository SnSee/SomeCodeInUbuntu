#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	int i = execlp("ls", "ls", NULL);
	printf("after execl\n");
	printf("i = %d\n", i);
	return 0;
}
