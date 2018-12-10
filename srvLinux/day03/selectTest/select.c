#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "wrap.h"

#define SERV_PORT 6666

int main(int argc, char *argv[])
{
	int i, j, n, maxi;

	int nready, client[FD_SETSIZE];                 /* 自定义数组client, 防止遍历1024个文件描述符  FD_SETSIZE默认为1024 */
	int maxfd, listenfd, connfd, sockfd;
	char buf[BUFSIZ], str[INET_ADDRSTRLEN];         /* #define INET_ADDRSTRLEN 16 */

	struct sockaddr_in clie_addr, serv_addr;
	socklen_t clie_addr_len;
	fd_set rset, allset;                            /* rset 读事件文件描述符集合 allset用来暂存 */

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family= AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port= htons(SERV_PORT);

	Bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	Listen(listenfd, 128);

	maxfd = listenfd;
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);

	while(1){
		rset = allset;
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);
		if(nready < 0){
			perr_exit("select err");
		}
		if(FD_ISSET(listenfd, &rset)){
			clie_addr_len = sizeof(clie_addr);
			connfd = Accept(listenfd, (struct sockaddr*)&clie_addr, &clie_addr_len); 
			FD_SET(connfd, &allset);
			if(maxfd < connfd)
				maxfd = connfd;
		}
		for(i = listenfd + 1; i <= maxfd; i++){
			int len_r = Read(connfd, buf, sizeof(buf));
			if(len_r == 0){
				Close(i);
				FD_CLR(i, &allset);
			}
			for(j = 0; j < len_r; j++){
				buf[j] = toupper(buf[j]);
			}	
			int len_w = Write(connfd, buf, len_r);
		}


	}

	Close(listenfd);
	return 0;
}   

