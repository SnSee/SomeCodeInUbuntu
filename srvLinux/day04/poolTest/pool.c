#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

#define BUFLEN 4096
#define SRV_PORT 8000
#define MAX_EVENTS 1024
/*
 * function: advanced usage of epool
 */
struct myevent_s{
	int fd;
	int events;
	void *arg;
	void (*callback)(int fd, int events, void *arg);
	int ontree;
	char buf[BUFLEN];
	int len;
	long time_last;
}

int efd_g;	//root file descriptor of red-black-tree
struct myevent_s events_s[MAX_EVENTS + 1];

//initialize member variables in structure myevent_s
void eventset(struct myevent_s *ev, int fd, void (*callback)(int,int,void*), void *arg){
	ev->fd = fd;
	ev->callback = callback;
	ev->events = 0;
	ev->arg = arg;
	ev->ontree = 0;
	memset(ev->buf, 0, sizeof(ev->buf));
	ev->len = 0;
	ev->time_last = time(NULL);

	return;
}
//add a file descriptor(which is in argument ev) into the red-black-tree
void eventadd(int efd, int events, struct myevent_s *ev){
	struct epoll_event epv = {0, {0}};
	int op;
	epv.data.ptr = ev;
	epv.events = ev->events = events;

	if(ev->ontree == 0){
		op = EPOLL_CTL_ADD;
		ev->ontree = 1;
	}
	if(epoll_ctl(efd, op, ev->fd, &epv) < 0)
		printf("epoll_ctl err in %s\n", __func__);
	
	return;
}
void init_listen_socket(int efd, int port){
	int lfd = Socket(AF_INET, SOCK_STREAM, 0);
	//set lfd status to nonblock
	int flags = fcntl(lfd, F_GETFL);
	flags |= O_NONBLOCK;
	fcntl(lfd, F_SETFL, flags);

	struct sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_port = htons(port);
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	Bind(lfd, (struct sockaddr*)&srv_addr, sizeof(srv_addr));
	Listen(lfd, 256);
	
}

void func1(int argc, char *argv[]){
	unsigned short port = SRV_PORT;
	if(argc == 2)
		port = argv[1];
	efd_g = epoll_create(MAX_EVENTS + 1);
	if(efd_g <= 0)
		printf("epoll_create err in %s\n", __func__);

	init_listen_socket(efd_g, port);
}
int main(int argc, char *argv[])
{
	func1(argc, argv);
	return 0;
}
