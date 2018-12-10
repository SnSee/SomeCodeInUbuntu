#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include "wrap.h"

#define SRV_IP "127.0.0.1"
#define SRV_PORT 8000
/*
 * function: create a client 
 */
void func1(){
	int lfd;
	int len_r, len_w;
	struct sockaddr_in cltaddr, srvaddr;
	char buf[1024];

	lfd = Socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(SRV_PORT);
	inet_pton(AF_INET, SRV_IP, &srvaddr.sin_addr.s_addr);
	Connect(lfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
	while(1){
		fgets(buf, sizeof(buf), stdin);
		len_r = strlen(buf) + 1;
		len_w = Write(lfd, buf, len_r);
		len_r = Read(lfd, buf, sizeof(buf));
		if(len_r == 0){
        	printf("server closed\n");
        	break;
        }
		Write(STDOUT_FILENO, buf, len_r);
	}
	close(lfd);
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
