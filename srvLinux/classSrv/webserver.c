//web服务端程序--使用epoll模型
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>

#include "pub.h"
#include "wrap.h"

int http_request(int cfd);

int main()
{
	//修改进程的工作目录
	//int chdir(const char *path);
	char workPath[256] = {0};
	sprintf(workPath, "%s/webpath", getenv("HOME"));
	printf("work directory:[%s]\n", workPath);
	
	chdir(workPath);
	
	//socket-setsockopt-bind
	int lfd = tcp4bind(9999, NULL);
	
	//设置监听
	Listen(lfd, 128);
	
	//创建一个epoll树, 得到一个树根epfd
	int epfd = epoll_create(1024);
	
	//将监听文件描述符lfd上epoll树
	struct epoll_event ev;
	ev.data.fd = lfd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	
	int i;
	int cfd;
	int sockfd;
	int nready;
	struct epoll_event events[1024];
	
	//进入while循环
	while(1)
	{
		nready = epoll_wait(epfd, events, 1024, -1);
		if(nready<0)
		{
			if(errno==EINTR) //若是被信号中断
			{
				printf("interuput by signo\n");
				continue;
			}
			else 
			{
				exit(-1);
			}
		}
		
		//下面是有事件发生的情况
		for(i=0; i<nready; i++)
		{
			sockfd = events[i].data.fd;
			
			//若是客户端有链接请求
			if(lfd==sockfd)
			{
				//接受新的客户端连接请求
				cfd = Accept(lfd, NULL, NULL);	
				
				//将新的文件描述符上epoll树
				ev.data.fd = cfd;
				ev.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);	
				
				//设置cfd为非阻塞模式
				int flag = fcntl(cfd, F_GETFL);
				flag |= O_NONBLOCK;
				fcntl(cfd, F_SETFL, flag);	
							
			}
			else //有客户端发送数据到来
			{
				//处理浏览器的请求数据
				http_request(sockfd);  
			}
		}
	}
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
int http_request(int cfd)
{
	int n;
	char buf[1024] = {0};
	char reqType[16] = {0};
	char fileName[256] = {0};
	char version[8] = {0};
	
	//读取请求行数据
	Readline(cfd, buf, sizeof(buf));
	printf("[%s]\n", buf);
	
	//解析出浏览器要访问的资源文件的名字
	//GET /hanzi.c HTTP/1.1
	sscanf(buf, "%[^ ] %[^ ] %[^ \r\n]", reqType, fileName, version);
	printf("[%s], [%s], [%s]\n", reqType, fileName, version);
	
	char *pFile = fileName+1;
	printf("pFile==[%s]\n", pFile);
	
	//将缓冲区中剩余的数据读完,避免产生粘包现象
	while((n=Read(cfd, buf, sizeof(buf)))>0);
	printf("----read over here\n");
	
	//判断文件释放存在
	struct stat st;
	int ret = stat(pFile, &st);
	printf("stat end here, ret==[%d]\n", ret);
	if(ret<0)//文件不存在
	{
		//发送应答头部信息
		//int send_header(int cfd, char *code, char *msg, int len, char *type)
		send_header(cfd, "404", "Not Found", 0, get_mime_type(".html"));
		
		//发送应答文件内容
		//int send_file(int cfd, char *fileName)
		send_file(cfd, "error.html");
		
	}
	else //文件存在
	{
		//发送应答头部信息
		send_header(cfd, "200", "OK", st.st_size, get_mime_type(pFile));
		
		//发送应答文件内容
		send_file(cfd, pFile);
	}
		
	return 0;
}
