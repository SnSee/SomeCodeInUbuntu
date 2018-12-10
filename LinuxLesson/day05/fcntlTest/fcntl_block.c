#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments");
//		return -1;
//	}
	int flag;
	//get flag
	//flag = fcntl(1, F_GETFL);	
	//change flag
	//flag = flag | O_NONBLOCK;
	//set flag
	//fcntl(1, F_SETFL, flag);

	char buf[1024];
	int len = read(1, buf, sizeof(buf));
	if(len < 0){
		printf("input nothing\n");
	}
	else{
		int len_w = write(1, buf, strlen(buf));
	}
	return 0;
}
