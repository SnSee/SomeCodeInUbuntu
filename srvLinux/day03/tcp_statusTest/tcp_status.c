#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "wrap.h"

#define SRV_IP "127.0.0.1"
#define SRV_PORT 8000
/*
 * function: simulate the TCP status of server
 */
void sys_err(const char *str)
{
	perror(str);
	exit(1);
}

void sig_child(int arg){
	while(waitpid(0, NULL, WNOHANG) > 0);
	return;
}
void tcp_status(char *buf, char *str){
	//remove '\n' in string buf
	buf[strlen(buf) - 1] = '\0';
	printf("tcp_status:\n");
	static int isconnect = 0;
	printf("buf = %s\n", buf);
	if(strcmp(buf, "SYN") == 0){
		printf("SYN_RCVD\n");
		strcpy(str, "SYN_ACK");
	}
	else if(strcmp(buf, "ACK") == 0){
		if(!isconnect){
			printf("ESTABLISHED\n");
			isconnect = 1;
		}
		else{
			printf("CLOSED\n");
			isconnect = 0;
		}
	}
	else if(strcmp(buf, "FIN") == 0){
		strcpy(str, "ACK");
		printf("CLOSE_WAIT\n");
		strcpy(str, "FIN_ACK");
		printf("LAST_ACK\n");
	}
	//recover string buf
	//buf[strlen(buf)] = '\n';
}
void func1(){
	struct sigaction newsig;
	newsig.sa_handler = sig_child;
	sigemptyset(&newsig.sa_mask);
	newsig.sa_flags = 0;
	sigaction(SIGCHLD, &newsig, NULL);

	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	int srv_ip;
	inet_pton(AF_INET, SRV_IP, &srv_ip);
	//srv_addr.sin_addr.s_addr = htonl(srv_ip);
	
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	int len_srv_addr = sizeof(srv_addr);
	Bind(lfd, (struct sockaddr *)&srv_addr, len_srv_addr);
	Listen(lfd, 256);

	//reuse port
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(opt));
	
	struct sockaddr_in clt_addr;
	int len_clt_addr = sizeof(clt_addr);

	int cfd;

	pid_t pid;
	while(1){
		cfd = Accept(lfd, (struct sockaddr*)&clt_addr, &len_clt_addr);
		pid = fork();
		if(pid == 0){
			break;
		}
		else if(pid < 0){
			perr_exit("fork err");
		}
		else
			continue;
	}
	while(1){
		char buf[BUFSIZ];
		char str[BUFSIZ];
		int len_r = read(cfd, buf, sizeof(buf));
		if(len_r < 0){
			perr_exit("read err");
		}
		else if(len_r == 0){
			printf("client closed\n");
			break;
		}
		//for(int i = 0; i < len_r; i++)
		//	buf[i] = toupper(buf[i]);
		
		//simulate the TCP status
		tcp_status(buf, str);

		int len_w = write(cfd, buf, len_r);
		if(len_w < 0){
			perr_exit("write err");
		}
		else if(len_w == 0){
			printf("client closed\n");
			break;
		}
	}
}
int main(int argc, char *argv[])
{
	func1();
	return 0;
}
