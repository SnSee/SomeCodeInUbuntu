#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include "wrap.h"
#define SRV_PORT 9000

/*
 * function:the server
 */
void sys_err(const char *str)
{
	perror(str);
	exit(1);
}
//function disposing signal SIGCHLD
void handler_sigchld(int signum){
	while(waitpid(-1, NULL, WNOHANG) > 0);
	return;
}	
	
int main(int argc, char *argv[])
{
	int cfd = -1;
	pid_t pid;
	//create a socket to monitor 
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//reuse port
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));
	Bind(lfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
	Listen(lfd, 128);

	
	
	
	//assign the function to dispose signal SIGCHLD
	struct sigaction act_new;
	act_new.sa_handler = handler_sigchld;
	sigemptyset(&act_new.sa_mask);
	act_new.sa_flags = 0;
	sigaction(SIGCHLD, &act_new, NULL);
	struct sockaddr_in clt_addr;
	socklen_t len_clt_addr = sizeof(clt_addr);
	while(1){
		cfd = Accept(lfd, (struct sockaddr*)&clt_addr, &len_clt_addr);
		if(cfd == -1){
			perr_exit("accept");
		}
		pid = fork();
		if(pid < 0){
			perr_exit("fork");
		}
		else if(pid == 0){
			close(lfd);
			break;
		}
		else if(pid > 0){
			close(cfd);
			continue;
		}
	}
	char buf[BUFSIZ];
	int len_r, len_w;
	while(1){
		len_r = read(cfd, buf, sizeof(buf));
		if(len_r < 0){
			perr_exit("read");
		}
		for(int i = 0; i < len_r; i++)
			buf[i] = toupper(buf[i]);

		len_w = write(cfd, buf, len_r);
		if(len_w < 0){
			perr_exit("write");
		}
	}	

	return 0;
}
