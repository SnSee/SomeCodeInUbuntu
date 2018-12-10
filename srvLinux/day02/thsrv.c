#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>
#include "wrap.h"

#define SRV_PORT 9000
/*
 * function: create a server using multi-thread mechanism
 */
void sys_err(const char *str)
{
	perror(str);
	exit(1);
}
//a structure including client address/port and communication socket file descriptor
struct add_fd{
	int cfd;
	struct sockaddr_in clt_addr;	
};
//the thread that communicates with a client
void *handler_srv_chi(void *arg){
	struct add_fd af = *(struct add_fd*)arg;

	//client ip address
	char str_ip[32];
	inet_ntop(AF_INET, &af.clt_addr.sin_addr.s_addr, str_ip, sizeof(str_ip));
	//client port number
	int num_port;
	num_port = ntohs(af.clt_addr.sin_port);

	char buf[BUFSIZ];
	int len_r, len_w;
	while(1){
		len_r = read(af.cfd, buf, sizeof(buf));	
		if(len_r < 0){
			printf("read error\n");
		}
		else if(len_r == 0){
			printf("client close\n");
			return NULL;             
		}                            
		//display client ip and port
		printf("%s:%d:\n", str_ip, num_port);
		//show message received from the client
		printf("\t%s", buf);
		for(int i = 0; i < len_r; i++){
			buf[i] = toupper(buf[i]);
		}
		len_w = write(af.cfd, buf, len_r);
		if(len_w < 0){                 
			printf("write error\n");  
		}
		else if(len_w == 0){
			printf("buffer full\n");
		}
	}
	return NULL;
}
int main(int argc, char *argv[])
{
	//the maximum of threads
	pthread_t tid[100];
	int i = 0;

	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(lfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
	Listen(lfd, 200);

	int cfd = -1;
	struct sockaddr_in clt_addr;
	struct add_fd af;
	socklen_t len_clt_addr = sizeof(clt_addr);
	while(1){
		cfd = Accept(lfd, (struct sockaddr*)&clt_addr, &len_clt_addr);
		af.cfd = cfd;
		af.clt_addr = clt_addr;
		
		pthread_create(&tid[i], NULL, handler_srv_chi, (void *)&af);
		pthread_detach(tid[i]);

		i++;
		if(i >= 100){
			printf("count of threads limit\n");
			//TODO
		}
	}

	return 0;
}
