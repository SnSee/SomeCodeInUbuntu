#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * function: encapsulate socket related functions
 */
void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

int Socket(int domain, int type, int protocol){
	int lfd = socket(domain, type, protocol);
	if(ret < 0){
		sys_err("socket err");
	}
	return lfd;
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen){
again:
	int cfd = accept(sockfd, sockaddr, addrlen);
	if(ret < 0){
		if(errno == ECONNABORTED || errno == EINTR){
			goto again;
		}
		sys_err("accept err");
	}
	return cfd;
}
