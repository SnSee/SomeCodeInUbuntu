#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <stddef.h>
#include "wrap.h"

/*
 * function: create a server communicating with local socket 
 */
void func1(){
	int cfd;
	int i, len_r, len_w;
	char buf[BUFSIZ];
	int sfd = Socket(AF_UNIX, SOCK_STREAM, 0);	
	struct sockaddr_un srvaddr, cltaddr;
	srvaddr.sun_family = AF_UNIX;
	strcpy(srvaddr.sun_path, "srv.socket");
	bzero(&cltaddr, sizeof(cltaddr));
	socklen_t len_cltaddr = sizeof(cltaddr);

	unlink("srv.socket");
	Bind(sfd, (struct sockaddr*)&srvaddr, offsetof(struct sockaddr_un, sun_path) + sizeof(srvaddr.sun_path));
	Listen(sfd, 10);
	cfd = Accept(sfd, (struct sockaddr*)&cltaddr, &len_cltaddr);
	while(1){
		len_r = Read(cfd, buf, sizeof(buf));	
		for(i = 0; i < len_r; i++)
			buf[i] = toupper(buf[i]);
		len_w = Write(cfd, buf, len_r);
	}
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
