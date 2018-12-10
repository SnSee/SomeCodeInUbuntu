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
 * function: create a client communicating with local socket
 */
void func1(){
	char buf[BUFSIZ];
	int len_r;
	struct sockaddr_un srvaddr;
	int sfd = Socket(AF_UNIX, SOCK_STREAM, 0);	
	struct sockaddr cltaddr = {
		AF_UNIX, "clt.socket"
	};
	srvaddr.sun_family = AF_UNIX;
	strcpy(srvaddr.sun_path, "srv.socket");

	unlink("clt.socket");
	Bind(sfd, (struct sockaddr*)&cltaddr, offsetof(struct sockaddr_un, sun_path) + sizeof(srvaddr.sun_path));

	connect(sfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
	while(1){
		len_r = Read(STDIN_FILENO, buf, sizeof(buf));
		Write(sfd, buf, len_r);
		Read(sfd, buf, sizeof(buf));
		Write(STDOUT_FILENO, buf, len_r);
	}
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
