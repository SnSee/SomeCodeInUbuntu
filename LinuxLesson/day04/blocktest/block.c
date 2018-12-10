#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
	char buf[100];
	int len_r = read(0, buf, sizeof(buf));
	if(len_r < 0){
		printf("read failed");
	}
	int len_w = write(1, buf, strlen(buf));
	if(len_w < 0){
		printf("write failed");
	}
	return 0;
}
