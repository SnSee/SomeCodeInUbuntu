#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include "wrap.h"

#define SRV_PORT 8000
/*
 * function: create a server whose protocol is UDP
 */
void func1(){
	int sfd;
	char buf[1024];
	int len_r, len_w;
	struct sockaddr_in cltaddr, srvaddr;
	int len_cltaddr = sizeof(cltaddr);
	char cltip[32];

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(SRV_PORT);
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);	

	sfd = Socket(AF_INET, SOCK_DGRAM, 0);
	Bind(sfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));

	while(1){
		len_r = recvfrom(sfd, buf, sizeof(buf), 0, (struct sockaddr*)&cltaddr, &len_cltaddr);
		if(len_r < 0){

		}
		inet_ntop(AF_INET, &cltaddr.sin_addr.s_addr, cltip, sizeof(cltip));
		buf[len_r] = '\0';
		printf("%s %d: %s", cltip, ntohs(cltaddr.sin_port), buf);
		char str[] = "message received\n";
		len_w = sendto(sfd, str, sizeof(str), 0, (struct sockaddr*)&cltaddr, sizeof(cltaddr));
		if(len_w < 0){
			printf("sendto failed\n");
		}
	}
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
