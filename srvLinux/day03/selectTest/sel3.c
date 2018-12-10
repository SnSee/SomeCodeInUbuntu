#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <sys/select.h>
#include "wrap.h"

#define SRV_PORT 8000
/*
 * function: create a server with select and use array to make it 
 * more efficient
 */
void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

void func1(){
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	//if the port SRV_PORT is used, reuse it
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEPORT, (void*)&opt, sizeof(opt));

	Bind(lfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
	Listen(lfd, 256);

	//the structure to store the ip and port of client
	struct sockaddr_in clt_addr;
	clt_addr.sin_family = AF_INET;

	//array to store socket fds that are active
	int clt_fds[FD_SETSIZE];	//FD_SETSIZE equals 1024
	int max_i = 0;					//record the maximum of valid fd in the array
	for(int i = 0; i < FD_SETSIZE; i++){
		clt_fds[i] = -1;
	}
	int maxfd = lfd;

	//the set of file descriptors
	fd_set set_r, set_r0;
	FD_ZERO(&set_r0);
	FD_SET(lfd, &set_r0);
	
	//count of corresponding sockets
	int count_fd = 0;
	int cfd;
	int len_clt_addr = sizeof(clt_addr);
	char buf[BUFSIZ];
	char str_ip[32];
	int num_port;
	while(1){
		set_r = set_r0;
		count_fd = select(maxfd + 1, &set_r, NULL, NULL, NULL);
		if(count_fd < 0)
			perr_exit("select err");
		if(FD_ISSET(lfd, &set_r)){	//indicate a new connection request
			cfd = Accept(lfd, (struct sockaddr*)&clt_addr, &len_clt_addr);
			printf("%s %d:\nconnected\n", 
					inet_ntop(AF_INET, &clt_addr.sin_addr.s_addr, str_ip, 32), 
					ntohs(clt_addr.sin_port));
			FD_SET(cfd, &set_r0);
			cfd > maxfd ? maxfd = cfd : 1;

			for(int i = 0; i < FD_SETSIZE; i++){
				if(clt_fds[i] < 0){
					clt_fds[i] = cfd;
					i > max_i ? max_i = i : 1;
					break;
				}
			}
			if(--count_fd == 0)
				continue;
		}	
		for(int i = 0; i <= max_i; i++){
			int fd_c;
			if((fd_c = clt_fds[i]) < 0)
				continue;
			if(FD_ISSET(fd_c, &set_r)){
				int len_r = Read(fd_c, buf, sizeof(buf));
				inet_ntop(AF_INET, &clt_addr.sin_addr.s_addr, str_ip, sizeof(str_ip));
				num_port = ntohs(clt_addr.sin_port);
				if(len_r == 0){
					printf("%s %d:\n", str_ip, num_port);
					printf("client closed\n");
					Close(fd_c);
					FD_CLR(fd_c, &set_r0);
					clt_fds[i] = -1;
				}
				else if(len_r > 0){
					for(int i = 0; i < len_r; i++)
						buf[i] = toupper(buf[i]);
					Write(fd_c, buf, len_r);
					printf("%s %d:\n", str_ip, num_port);
					Write(STDOUT_FILENO, buf, len_r);
				}
				if(--count_fd == 0)
					break;

			}
		}	

	}
}
int main(int argc, char *argv[])
{
	func1();
	return 0;
}
