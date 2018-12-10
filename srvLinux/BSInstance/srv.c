#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include "wrap.h"
#include "srv.h"
#include "pub.h"

#define MAX_NODES 1024
#define SRV_PORT 8000
//#define HTMLS "./htmls/"
#define ERROR_PAGE "./htmls/error_404.html"
/*
 * function: create a server communicate with browser
 */
void func1(){
	int lfd, cfd, efd, sockfd;
	int i, j, nready;
	struct epoll_event ev, evs[MAX_NODES];
	struct sockaddr_in srvaddr = {
		AF_INET, htons(SRV_PORT), htonl(INADDR_ANY)
	}, cltaddr;
	int len_cltaddr = sizeof(cltaddr);	
	
	lfd = Socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, (void*)&opt, sizeof(opt));
	Bind(lfd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
	Listen(lfd, 20);

	efd = epoll_create(MAX_NODES);
	ev.events = EPOLLIN;
	ev.data.fd = lfd;
	epoll_ctl(efd, EPOLL_CTL_ADD, lfd, &ev);

	while(1){
		printf("---start wait---\n");
		nready = epoll_wait(efd, evs, MAX_NODES, -1);
		if(nready < 0){
			printf("epoll_wait error\n");
			exit(-1);
		}
		for(i = 0; i < nready; i++){
			sockfd = evs[i].data.fd;
			if(lfd == sockfd){
				cfd = Accept(lfd, NULL, NULL);
				printf("\nnew Connection\n\n");

				//set the file status flag to nonblock mode
				int flag = fcntl(cfd, F_GETFL);
				flag |= O_NONBLOCK;
				fcntl(cfd, F_SETFL, flag);

				//add the file descriptor to the tree
				ev.events = EPOLLIN;
				ev.data.fd = cfd;
				epoll_ctl(efd, EPOLL_CTL_ADD, cfd, &ev);
			}
			else{
				//respond to the request received from the browser
				request_respond(efd, sockfd); 
				printf("after request_respond\n");
				printf("--------------------------\n");
				//close the file descriptor after sending data
				//or the browser will wait for more data
				close(cfd);
				epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
			}
		}
		//printf("exit loop\n");
	}
	printf("out of while(1)\n");
}
//the function responding to the request
void request_respond(int efd, int cfd){
	char buf[BUFSIZ] = "";
	char req_type[16] = "";
	char req_fileName[256] = "";
	char req_http_version[16] = "";
	char fileName[256] = "";
	char fileName_tmp[256] = "";

	int len_r = Readline(cfd, buf, sizeof(buf));
	if(len_r == 0){
		printf("web page closed\n");
		close(cfd);
		epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
		return;
	}
	//parse the first line of request
	sscanf(buf, "%[^ ] %[^ ] %[^ \r]\r\n", req_type, req_fileName, 
			req_http_version);

	char *pFileName = req_fileName;
	if(strlen(pFileName) <= 1){
		strcpy(pFileName, "./");
	}
	else{
		pFileName++;
	}
	printf("type[%s] fileName[%s] ver[%s]\n", req_type, pFileName, req_http_version);

	//printf("before read\n");
	//printf("after read\n");
	//将缓冲区中剩余的数据读完,避免产生粘包现象
	int n = 0;
	while((n=Readline(cfd, buf, sizeof(buf))) > 0);

	char cur_path[64] = "";
	getcwd(cur_path, 64);
	cur_path[strlen(cur_path)] = '/';
	strcpy(fileName, pFileName);
	if(strcmp(pFileName, "./") != 0){
		//strcpy(fileName_tmp, cur_path);
		//strcat(fileName_tmp, pFileName);
		strcpy(fileName_tmp, pFileName);
		strcpy(fileName, fileName_tmp);
	}

	printf("1 fileName = [%s]\n", fileName);
	//printf("fileName = %s\n", fileName);
	//detect whether the file is valid
	struct stat st;
	int ret_stat = stat(fileName, &st);
	if(ret_stat < 0){
		perror("stat err");
		printf("after perror stat\n");
		//strcpy(fileName_tmp, HTMLS);
		strcat(fileName_tmp, ERROR_PAGE);
		strcpy(fileName, fileName_tmp); 
		printf("**fileName = %s\n", fileName);
		stat(fileName, &st);
		sendheader(cfd, "HTTP/1.1", "404 Not Found", st.st_size,
			"text/html;charset=utf8");
		senddata(cfd, fileName);
	}
	else{
		//send different data according it's a normal file or
		//a directory
		//if it's a directory
		if(S_ISDIR(st.st_mode)){
			sendheader(cfd, "HTTP/1.1", "200 OK", 0,
					"text/html;charset=utf8");
			//
			struct dirent **namelist;
			int n;
			char subfile[1024]= "";
			n = scandir(fileName, &namelist, NULL, alphasort);
			if(n == -1){
				perror("scandir error");
				exit(-1);
			}
			senddata(cfd, "./htmls/html_header.html"); 
			sprintf(subfile, "all file reachable");
			Writen(cfd, subfile, strlen(subfile));
			//char cur_path[256] = "";
			//getcwd(cur_path, sizeof(cur_path));
			//printf("cur_path = %s\n", cur_path);
			while(n--){
				//avoid displaying invisiable files
				if(*(namelist[n]->d_name) == '.')
					continue;
				printf("namelist[%d] = %s\n", n+1, namelist[n]->d_name);
				//if namelist[n] is a subdirectory
				if(namelist[n]->d_type == DT_DIR){
					sprintf(subfile, "<li><a href=%s/ title=\"%s\" color=\"#0000ff\">%s</a></font></li>",
						namelist[n]->d_name, 
						namelist[n]->d_name, 
						namelist[n]->d_name);
				}
				else{
					sprintf(subfile, "<li><font color=\"#00ff00\">"
							"<a href=%s color=\"#00ff00\">%s</a></li></font>",
						namelist[n]->d_name, namelist[n]->d_name);
				}
				Writen(cfd, subfile, strlen(subfile));
				free(namelist[n]);
			}
			free(namelist);
			senddata(cfd, "./htmls/html_footer.html");
			close(cfd);
			epoll_ctl(efd, EPOLL_CTL_DEL, cfd, NULL);
		}
		//if it's a normal file
		else if(S_ISREG(st.st_mode)){
			sendheader(cfd, "HTTP/1.1", "200 OK", st.st_size,
				get_mime_type(fileName));
			//sendheader(cfd, "HTTP/1.1", "200 OK", st.st_size,
				//"text/plain;charset=utf8");
			senddata(cfd, fileName);
		}
		//if it's of other file type
		else{
			//strcpy(fileName_tmp, HTMLS);
            strcat(fileName_tmp, ERROR_PAGE);
            strcpy(fileName, fileName_tmp); 
            printf("**fileName = %s\n", fileName);
            stat(fileName, &st);
			sendheader(cfd, "HTTP/1.1", "404 Not Found", st.st_size,
            	"text/html;charset=utf8");
            senddata(cfd, fileName);
		}
	}
	
	printf("**file_respond:[%s]\n", fileName);


	return;
}
//send the header
void sendheader(int cfd, char *http_version, char *status, int mess_len,
	char *mess_type){
	//printf("**enter sendheader**\n");
	char buf[BUFSIZ] = "";
	sprintf(buf, "%s %s\r\n", http_version, status);
	sprintf(buf + strlen(buf), "Content-Type:%s\r\n", mess_type);
	if(mess_len > 0){
		sprintf(buf + strlen(buf), "Content-Length:%d\r\n", mess_len);
	}
	strcat(buf, "\r\n");

	printf("**sendheader: buf = %s\n", buf);
	//Writen(cfd, buf, strlen(buf) + 1);
	Writen(cfd, buf, strlen(buf));
	//printf("**exit sendheader**\n");
	return;
}

int getdata(int fd, char *buf, int len){
	int len_r = Read(fd, buf, len);
	return len_r;
}

void senddata(int cfd, char *fileName){
	char buf[BUFSIZ] = "";
	int len_r;
	int fd = open(fileName, O_RDONLY);
	if(fd < 0){
		perror("open error");
		printf("fileName = [%s]\n", fileName);
		return;	
	}
	while(1){
		len_r = Readline(fd, buf, sizeof(buf));
		if(len_r == 0)
			break;
		printf("+buf = [%s]\n", buf);
		Writen(cfd, buf, len_r);
	}
	close(fd);
	return;	
}

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler = SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGPIPE, &act, NULL);

	char *workpath = (char *)malloc(32);
	//change the work path
	strcpy(workpath, getenv("HOME"));
	if(NULL == workpath){
		perror("chdir error");
		free(workpath);
		exit(-1);
	}
	strcat(workpath, "/srvLinux");	
	chdir(workpath);
	printf("the working path is %s\n", workpath);
	char buf[32] = "";
	getcwd(buf, 32);
	printf("cur_path = %s\n", buf);
	free(workpath);
	func1();

	return 0;
}
