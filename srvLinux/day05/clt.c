#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <arpa/inet.h>
#include "wrap.h"

#define SRV_PORT 8000
#define SRV_IP "127.0.0.1"
/*
 * function: create a client with UDP protocol 
 */

void func1(){
	int srv_ip;
	int len_r;
	char buf[BUFSIZ];
	int sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
	struct sockaddr_in clt_addr, srv_addr;
	bzero(&srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	inet_pton(AF_INET, SRV_IP, &srv_addr.sin_addr.s_addr);
	
	while(1){
		fgets(buf, sizeof(buf), stdin);
		len_r = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
		recvfrom(sockfd, buf, sizeof(buf), 0, NULL, 0);
		write(STDOUT_FILENO, buf, len_r);
	}
	close(sockfd);
	return;	
	
}
int main(int argc, char *argv[])
{
	func1();

	return 0;
}
