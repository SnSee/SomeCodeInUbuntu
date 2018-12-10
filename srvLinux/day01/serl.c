#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define SRV_PORT 9875 

//function: estiblish a server
//error dispose
void err_disp(char *info){
	perror("info");
	exit(0);
}
int main(int argc, char *argv[]){
	//create an endpoint by function socket()
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if(lfd == -1)
		err_disp("socket");
	//create a variable of type struct sockaddr_in
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(SRV_PORT);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//bind the socket with ip address and port number
	int ret_bind = bind(lfd, (struct sockaddr *)&srv_addr, sizeof(srv_addr));
	if(ret_bind == -1)
		err_disp("bind");

	int ret_listen = listen(lfd, 10);

	//wait for a call from a client
	struct sockaddr_in clt_addr;
	int clt_len = sizeof(clt_addr);
	int cfd = accept(lfd, (struct sockaddr*)&clt_addr, &clt_len);
	if(-1 == cfd)
		err_disp("accept");

	char buf[1024];
	while(1){
		//read from the socket returned by accept()
		int len_r = read(cfd, buf, sizeof(buf));
		if(len_r < 0)
			err_disp("read");
		else if(len_r == 0){
			printf("client exited\n");
			close(cfd);
			break;
		}

		//print the ip and port of the client sending message
		char clt_ip[1024];
		inet_ntop(AF_INET, &clt_addr.sin_addr.s_addr, clt_ip, sizeof(clt_ip));
		printf("%s:%d:\n", clt_ip, ntohs(clt_addr.sin_port));
		int len_w = write(1, buf, len_r);
		if(len_w < 0)
			err_disp("write");

		for(int i = 0; i < len_r; i++)
			buf[i] = toupper(buf[i]);
		//respond to client
		write(cfd, buf, len_r);
	}

	close(lfd);
	//close(cfd);

	return 0;
}
