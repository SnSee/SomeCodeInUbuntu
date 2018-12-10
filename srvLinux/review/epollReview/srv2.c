#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>
#include <ctype.h>
#include <fcntl.h>
#include "wrap.h"

#define SRV_PORT 8000
#define MAX_EVENTS 1024
/*
 * function: create a server with epoll()
 * and set the event mode to Edge Treggered
 */
void func1(){
	int ep, lfd, cfd;
	int i, j;	 
	int n_cor, len_r, len_w;
	char buf[1024];
	struct epoll_event ev, evs[MAX_EVENTS];
	struct sockaddr_in srvaddr ={
		AF_INET, htons(SRV_PORT), htonl(INADDR_ANY) 
	}, cltaddr;
	int len_cltaddr = sizeof(cltaddr);

	lfd = Socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));

	Bind(lfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
	Listen(lfd, 100);
	ep = epoll_create(100);
	ev.events = EPOLLIN|EPOLLET;
	ev.data.fd = lfd;
	epoll_ctl(ep, EPOLL_CTL_ADD, lfd, &ev);

	int flag = fcntl(lfd, F_GETFD);
	flag |= O_NONBLOCK;
	fcntl(lfd, F_SETFD, flag);

	while(1){
		n_cor = epoll_wait(ep, evs, MAX_EVENTS, -1);
		if(n_cor < 0){
			perror("epoll_wait err");
			break;
		}
		for(i = 0; i < n_cor; i++){
			if(evs[i].data.fd == lfd){
				cfd = Accept(lfd, (struct sockaddr*)&cltaddr, &len_cltaddr);
				ev.events = EPOLLIN|EPOLLET;
				ev.data.fd = cfd;
				epoll_ctl(ep, EPOLL_CTL_ADD, cfd, &ev);
			}
			else{
				len_r = Read(evs[i].data.fd, buf, sizeof(buf));
				if(len_r == 0){
					printf("client closed\n");
					Close(evs[i].data.fd);
					epoll_ctl(ep, EPOLL_CTL_DEL, evs[i].data.fd, NULL);
					continue;
				}
				for(j = 0; j < len_r; j++){
					buf[j] = toupper(buf[j]);
				}
				len_w = Write(evs[i].data.fd, buf, len_r);

			}
		}
	}
	close(lfd);
	return;
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
