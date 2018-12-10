#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define CLIENT_PORT 9875

int main(int argc, char *argv[]){

	int cfd = socket(AF_INET, SOCK_STREAM, 0);
	if(cfd < 0){
		printf("socket error\n");
	}
	struct sockaddr_in clt_addr;
	clt_addr.sin_family = AF_INET;
	clt_addr.sin_port = htons(CLIENT_PORT);
	clt_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int ret_con = connect(cfd, (struct sockaddr*)&clt_addr, sizeof(clt_addr));
	if(ret_con < 0){
		printf("connect error\n");
		exit(-1);
	}
	else{
		printf("connected\n");
	}
	char buf[BUFSIZ];
	while(1){
		int len_r = read(0, buf, sizeof(buf));
		if(len_r < 0){
			printf("read error\n");
			break;
		}
		
		int len_w = write(cfd, buf, len_r);
		if(len_w < 0){
			printf("write error\n");
			break;
		}

		int len_rr = read(cfd, buf, sizeof(buf));
		if(len_rr < 0){
			printf("read from cfd error\n");
			break;
		}

		int len_ww = write(1, buf, len_rr);
		if(len_ww < 0){
			printf("write cfd error\n");
			break;
		}
	}
	close(cfd);
	return 0;
}
