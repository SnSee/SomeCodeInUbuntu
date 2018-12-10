#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

/*
 * function: set alarm with alarm()
 * usage: ./alarm
 */
int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments\n");
//		return -1;
//	}
	int ret = alarm(5);
	int fd = open("/dev/tty", O_RDWR|O_NONBLOCK);
	//int fd = open("/dev/tty", O_RDWR);
	int num = 5;
	char buf[10];
	while(1){
		printf("%d\n", num--);
		int len_r = read(fd, buf, 4);
		//printf("buf[0] = %c\n", buf[0]);
		if('a' == buf[0]){
			alarm(0);
			printf("alarm canceled\n");
		}
		else if('b' == buf[0]){
			int left = alarm(5);
			printf("%d sec left, 5 sec set\n", left);
		}
		//printf("alive\n");
		sleep(1);
	}
	return 0;
}
