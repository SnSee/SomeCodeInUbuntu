#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <ctype.h>
#include "wrap.h"

#define MAX_FD 100
#define SRV_PORT 8000
/*
 * function: create a server with select()
 */
void func1(){
	int lfd, cfd, maxfd, fds[MAX_FD];
	int fds_i = 0;
	int num_cor = 0, len_r, len_w;
	int len_cltaddr = 0;
	int i, j, maxi;
	char cltip[32], buf[1024];
	unsigned short cltport = 0;
	struct sockaddr_in cltaddr;
	struct sockaddr_in srvaddr = {
		AF_INET, htons(SRV_PORT), htonl(INADDR_ANY)
	};
	len_cltaddr = sizeof(cltaddr);

	lfd = Socket(AF_INET, SOCK_STREAM, 0);	
	maxfd = lfd;
	fds[MAX_FD - 1] = lfd;
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
	
	Bind(lfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
	Listen(lfd, 20);

	maxi = -1;
	for(i = 0; i < 100; i++){
		fds[i] = -1;
	}

	fd_set set_r, set_tmp;
	FD_ZERO(&set_r);
	FD_SET(lfd, &set_r);	
	while(1){
		set_tmp = set_r;
		num_cor = select(maxfd + 1, &set_tmp, NULL, NULL, NULL);
		//printf("num_cor = %d\n", num_cor);
		if(num_cor < 0){
			perror("select err");
			return;
		}
		if(FD_ISSET(lfd, &set_tmp)){
			cfd = Accept(lfd, (struct sockaddr*)&cltaddr, &len_cltaddr);
			maxfd < cfd ? maxfd = cfd : 0;
			fds[fds_i++] = cfd;
			for(i = 0; i < 100; i++){
				if(fds[i] < 0){
					fds[i] = cfd;
					i > maxi ? maxi = i : 0;
					break;
				}
			}
			FD_SET(cfd, &set_r);
			inet_ntop(AF_INET, &cltaddr.sin_addr.s_addr, cltip, sizeof(cltip));
			cltport = ntohs(cltaddr.sin_port);
			printf("%s %d: connected\n", cltip, cltport);
			if(--num_cor == 0)
				continue;
		}
		for(i = 0; i < maxi; i++){
			if(fds[i] < 0){
				continue;
			}
			if(FD_ISSET(fds[i],&set_tmp)){
				len_r = Read(fds[i], buf, sizeof(buf));
				inet_ntop(AF_INET, &cltaddr.sin_addr.s_addr, cltip, sizeof(cltip));
				cltport = ntohs(cltaddr.sin_port);
				if(0 == len_r){
					printf("i = %d, %s %d: closed\n", i, cltip, cltport);
					close(fds[i]);
					FD_CLR(fds[i], &set_r);
					fds[i] = -1;
					--num_cor;
					++i;
					continue;
				}
				buf[len_r] = '\0';
				printf("%s %d: %s", cltip, cltport, buf);
				for(j = 0; j < len_r; j++){
					buf[j] = toupper(buf[j]);
				}
				len_w = Write(fds[i], buf, len_r);
				--num_cor;
			}
		}	
	}
	close(lfd);
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
