#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <arpa/inet.h>
#include "wrap.h"

#define SRV_PORT 8000
/*
 * function: create a server with epoll()
 */
void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

void func1(){
	int lfd, cfd, efd;
	int ncor, i, j;
	int len_clt_addr, len_r, len_w;
	int clt_port;
	char buf[BUFSIZ], clt_ip[32] = "default";
	struct sockaddr_in srv_addr, clt_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	struct epoll_event e_node, e_array[5000];

	lfd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));

	Bind(lfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
	Listen(lfd, 256);

	efd = epoll_create(5000);
	if(efd < 0)
		perr_exit("epoll_create");

	e_node.events = EPOLLIN;
	e_node.data.fd = lfd;
	epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &e_node);
	while(1){
		ncor = epoll_wait(efd, e_array, 5000, -1);	
		if(ncor < 0)
			perr_exit("epoll_wait");

		for(i = 0; i < ncor; i++){
			if(e_array[i].data.fd == lfd){
				cfd = Accept(lfd, (struct sockaddr*)&clt_addr, &len_clt_addr);
				e_node.events = EPOLLIN;
				e_node.data.fd = cfd;
				epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &e_node);
				inet_ntop(AF_INET,
                     &clt_addr.sin_addr.s_addr, clt_ip, sizeof(clt_ip));
                clt_port = ntohs(clt_addr.sin_port);
                len_clt_addr = sizeof(clt_addr);
				printf("%s %d connected\n", clt_ip, clt_port); 
			}
			else{
				len_r = Read(e_array[i].data.fd, buf, sizeof(buf));
				if(len_r == 0){
					printf("%s %d:\n", clt_ip, clt_port); 
					printf("client closed\n");
					Close(e_array[i].data.fd);
					epoll_ctl(efd, EPOLL_CTL_DEL, e_array[i].data.fd, NULL);
				}
				else{
				//	printf("%s %d:\n", clt_ip, clt_port); 
					printf("%s %d:\n",inet_ntop(AF_INET,
                                           &clt_addr.sin_addr.s_addr, clt_ip, sizeof(clt_ip)),
                                      ntohs(clt_addr.sin_port));

					Write(STDOUT_FILENO, buf, len_r);
					buf[i] = toupper(buf[i]);
					len_w = Write(e_array[i].data.fd, buf, len_r);
				}
			}
		}
		
		

	}
}
int main(int argc, char *argv[])
{
	func1();
	return 0;
}
