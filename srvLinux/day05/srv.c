#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <string.h>
#include "wrap.h"

#define SRV_PORT 8000
/*
 * function: create a server with UDP protocol
 */

void func1(){
	int i;
	int len_r, len_s;
	char buf[BUFSIZ];
	int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in srv_addr, clt_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	bzero(&clt_addr, sizeof(clt_addr));
	socklen_t len_clt_addr = sizeof(clt_addr);
	Bind(sockfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));

	while(1){
		len_r = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&clt_addr, &len_clt_addr);
		for(i = 0; i < len_r; i++)
			buf[i] = toupper(buf[i]);
		len_s = sendto(sockfd, buf, len_r, 0, (struct sockaddr*)&clt_addr, sizeof(clt_addr));	
	}
	close(sockfd);	
}
int main(int argc, char *argv[])
{
	func1();

	return 0;
}
