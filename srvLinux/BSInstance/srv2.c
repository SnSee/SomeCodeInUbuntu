#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <dirent.h>
#include "wrap.h"
#include "pub.h"
#include "srv.h"

#define MAX_NODES 1024
#define SRV_PORT 8000
#define HTMLS "./srvLinux/htmls/"
#define ERROR_PAGE "error_404.html"
/*
 * function: create a server communicate with browser
 */

int http_request(int cfd, int epfd);
void request_respond(int efd, int cfd){
	printf("%%%%%%%%%%%%nothing%%%%%%%%%%%%%%\n");
	return;
}

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
		printf("---enter while(1)---\n");
		nready = epoll_wait(efd, evs, MAX_NODES, -1);
		if(nready < 0){
			printf("epoll_wait error\n");
			exit(-1);
		}
		for(i = 0; i < nready; i++){
			sockfd = evs[i].data.fd;
			if(lfd == sockfd){
				cfd = Accept(lfd, NULL, NULL);

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
				http_request(sockfd, efd); 
				printf("after request_respond\n");
			}
		}
		printf("out of for loop\n");
	}
	printf("out of while(1)\n");
}
//组织http协议应答数据---头部信息
int send_header(int cfd, char *code, char *msg, int len, char *type)
{
	char buf[1024] = {0};
	sprintf(buf, "HTTP/1.1 %s %s\r\n", code, msg);
	sprintf(buf+strlen(buf), "Content-Type: %s\r\n", type);
	if(len>0)
	{
		sprintf(buf+strlen(buf), "Content-Length: %d\r\n", len);
	}
	strcat(buf, "\r\n");
	
	printf("send_header: [%s]\n", buf);
	
	Writen(cfd, buf, strlen(buf));
	
	return 0;		
}

//发送文件内容
int send_file(int cfd, char *fileName)
{
	int n;
	char buf[1024];
	
	//打开文件
	int fd = open(fileName, O_RDONLY);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}
	
	while(1)
	{
		//读文件
		n = Readn(fd, buf, sizeof(buf));
		if(n<=0)
		{
			break;
		}
		
		//发送数据
		Writen(cfd, buf, n);
		printf("send_file: [%s], [%d]\n", buf, n);
	}
	
	close(fd);
	
	return 0;
}
//处理客户端请求
int http_request(int cfd, int epfd)
{
	int n;
	char buf[1024] = {0};
	char reqType[16] = {0};
	char fileName[256] = {0};
	char version[8] = {0};
	
	printf("--------------------\n");
	//读取请求行数据
	n = Readline(cfd, buf, sizeof(buf));
	if(n<=0)
	{
		close(cfd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, NULL);
		printf("client error or client,n==[%d]\n", n);
		return -1;
	}
	printf("[%s]\n", buf);
	
	//解析出浏览器要访问的资源文件的名字
	//GET /hanzi.c HTTP/1.1
	sscanf(buf, "%[^ ] %[^ ] %[^ \r\n]", reqType, fileName, version);
	printf("[%s], [%s], [%s]\n", reqType, fileName, version);
	
	//char *pFile;
	char *pFile = fileName;
	if(strlen(fileName)<=1)
	{
		//pFile = "./";
		strcpy(pFile, "./");
	}
	else 
	{
		pFile = fileName+1;
	}
	printf("pFile==[%s]\n", pFile);
	
	//将缓冲区中剩余的数据读完,避免产生粘包现象
	//while((n=Read(cfd, buf, sizeof(buf)))>0);
	//while((n=Readline(cfd, buf, sizeof(buf)))>0);
	while(1)
	{
		memset(buf, 0x00, sizeof(buf));
		n = Readline(cfd, buf, sizeof(buf));
		//printf("buf==[%s], n==[%d]\n", buf, n);	
		if(n<=0)
		{
			printf("read over\n");
			break;
		}		
	}
	
	//判断文件释放存在
	struct stat st;
	int ret = stat(pFile, &st);
	if(ret<0)//文件不存在
	{
		//发送应答头部信息
		send_header(cfd, "404", "Not Found", 0, get_mime_type(".html"));
		
		//发送应答文件内容
		send_file(cfd, "error.html");
		
	}
	else //文件存在
	{
		//如果为普通文件
		if(S_ISREG(st.st_mode))
		{
			//发送应答头部信息
			send_header(cfd, "200", "OK", st.st_size, get_mime_type(pFile));
			
			//发送应答文件内容
			send_file(cfd, pFile);
		}
		//如果为目录文件
		else if(S_ISDIR(st.st_mode))
		{
			
			//发送应答头部信息
			send_header(cfd, "200", "OK", 0, get_mime_type(".html"));
			
			//发送html文件头部
			send_file(cfd, "html/dir_header.html");
			
			//下面是发送文件列表内容
			struct dirent **namelist;
			int num;

			num = scandir(pFile, &namelist, NULL, alphasort);
			if (num < 0)
			{
			   perror("scandir");
			   return -1;
			}
			else 
			{
			   while(num--) 
			   {
			       printf("%s\n", namelist[num]->d_name);
			       memset(buf, 0x00, sizeof(buf));
			       if(namelist[num]->d_type==DT_DIR)
			       {
			       		sprintf(buf, "<li> <a href=%s/> %s </a> </li>", namelist[num]->d_name, namelist[num]->d_name);			       
			       }
			       else 
			       {
			       		sprintf(buf, "<li> <a href=%s> %s </a> </li>", namelist[num]->d_name, namelist[num]->d_name);			       
			       }
			       free(namelist[num]);
			       Writen(cfd, buf, strlen(buf));
			   }
			   free(namelist);
			}
			
			//发送html文件尾部
			send_file(cfd, "html/dir_tail.html");
			
			close(cfd);
		}
	}
		
	return 0;
}

int main(int argc, char *argv[])
{
	char *workpath = (char *)malloc(32);
	//change the work path
	strcpy(workpath, getenv("HOME"));
	if(NULL == workpath){
		perror("chdir error");
		free(workpath);
		exit(-1);
	}
	chdir(workpath);
	//printf("the working path is %s\n", workpath);
	free(workpath);
	func1();

	return 0;
}
