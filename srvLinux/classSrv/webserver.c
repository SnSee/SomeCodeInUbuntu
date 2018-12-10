//web����˳���--ʹ��epollģ��
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
	//�޸Ľ��̵Ĺ���Ŀ¼
	//int chdir(const char *path);
	char workPath[256] = {0};
	sprintf(workPath, "%s/webpath", getenv("HOME"));
	printf("work directory:[%s]\n", workPath);
	
	chdir(workPath);
	
	//socket-setsockopt-bind
	int lfd = tcp4bind(9999, NULL);
	
	//���ü���
	Listen(lfd, 128);
	
	//����һ��epoll��, �õ�һ������epfd
	int epfd = epoll_create(1024);
	
	//�������ļ�������lfd��epoll��
	struct epoll_event ev;
	ev.data.fd = lfd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	
	int i;
	int cfd;
	int sockfd;
	int nready;
	struct epoll_event events[1024];
	
	//����whileѭ��
	while(1)
	{
		nready = epoll_wait(epfd, events, 1024, -1);
		if(nready<0)
		{
			if(errno==EINTR) //���Ǳ��ź��ж�
			{
				printf("interuput by signo\n");
				continue;
			}
			else 
			{
				exit(-1);
			}
		}
		
		//���������¼����������
		for(i=0; i<nready; i++)
		{
			sockfd = events[i].data.fd;
			
			//���ǿͻ�������������
			if(lfd==sockfd)
			{
				//�����µĿͻ�����������
				cfd = Accept(lfd, NULL, NULL);	
				
				//���µ��ļ���������epoll��
				ev.data.fd = cfd;
				ev.events = EPOLLIN;
				epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);	
				
				//����cfdΪ������ģʽ
				int flag = fcntl(cfd, F_GETFL);
				flag |= O_NONBLOCK;
				fcntl(cfd, F_SETFL, flag);	
							
			}
			else //�пͻ��˷������ݵ���
			{
				//�������������������
				http_request(sockfd);  
			}
		}
	}
}

//��֯httpЭ��Ӧ������---ͷ����Ϣ
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

//�����ļ�����
int send_file(int cfd, char *fileName)
{
	int n;
	char buf[1024];
	
	//���ļ�
	int fd = open(fileName, O_RDONLY);
	if(fd<0)
	{
		perror("open error");
		return -1;
	}
	
	while(1)
	{
		//���ļ�
		n = Readn(fd, buf, sizeof(buf));
		if(n<=0)
		{
			break;
		}
		
		//��������
		Writen(cfd, buf, n);
		printf("send_file: [%s], [%d]\n", buf, n);
	}
	
	close(fd);
	
	return 0;
}
//����ͻ�������
int http_request(int cfd)
{
	int n;
	char buf[1024] = {0};
	char reqType[16] = {0};
	char fileName[256] = {0};
	char version[8] = {0};
	
	//��ȡ����������
	Readline(cfd, buf, sizeof(buf));
	printf("[%s]\n", buf);
	
	//�����������Ҫ���ʵ���Դ�ļ�������
	//GET /hanzi.c HTTP/1.1
	sscanf(buf, "%[^ ] %[^ ] %[^ \r\n]", reqType, fileName, version);
	printf("[%s], [%s], [%s]\n", reqType, fileName, version);
	
	char *pFile = fileName+1;
	printf("pFile==[%s]\n", pFile);
	
	//����������ʣ������ݶ���,�������ճ������
	while((n=Read(cfd, buf, sizeof(buf)))>0);
	printf("----read over here\n");
	
	//�ж��ļ��ͷŴ���
	struct stat st;
	int ret = stat(pFile, &st);
	printf("stat end here, ret==[%d]\n", ret);
	if(ret<0)//�ļ�������
	{
		//����Ӧ��ͷ����Ϣ
		//int send_header(int cfd, char *code, char *msg, int len, char *type)
		send_header(cfd, "404", "Not Found", 0, get_mime_type(".html"));
		
		//����Ӧ���ļ�����
		//int send_file(int cfd, char *fileName)
		send_file(cfd, "error.html");
		
	}
	else //�ļ�����
	{
		//����Ӧ��ͷ����Ϣ
		send_header(cfd, "200", "OK", st.st_size, get_mime_type(pFile));
		
		//����Ӧ���ļ�����
		send_file(cfd, pFile);
	}
		
	return 0;
}
