#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>

#define SRV_PORT 9876
#define SRV_IP "127.0.0.1"
/*
 * function: the client created with libevent 
 */
//
void read_cb(struct bufferevent *bev, void *arg){
	char buf[1024];
	bufferevent_read(bev, buf, sizeof(buf));
	printf("server:");
	//write(STDOUT_FILENO, buf, strlen(buf) + 1);
	bufferevent_write(bev, buf, strlen(buf) + 1);
	return;
}
//
void write_cb(struct bufferevent *bev, void *arg){
	printf("message send\n");
	return;	
}
//
void event_cb(struct bufferevent *bev, short what, void *arg){
	if(what & BEV_EVENT_EOF){
		printf("server closed\n");
	}
	else if(what & BEV_EVENT_ERROR){
		printf("unknown error\n");
	}
	else if(what & BEV_EVENT_CONNECTED){
		printf("connected with server\n");
		/////////////////////////////////////////////////
		//!!!must return here
		return;
	}
	bufferevent_free(bev);
	return;
}
void read_terminal(evutil_socket_t fd, short what, void *arg){
	char buf[1024];
	int len_r = read(fd, buf, sizeof(buf));
	bufferevent_write((struct bufferevent*)arg, buf, len_r + 1);
	return;
}
void func1(){
	struct event_base *base;
	struct bufferevent *bev;
	struct event *ev;
	struct sockaddr_in srvaddr;
	int fd;

	base = event_base_new();
	fd = socket(AF_INET, SOCK_STREAM, 0);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons(SRV_PORT);
	inet_pton(AF_INET, SRV_IP, &srvaddr.sin_addr.s_addr);

	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);

	bufferevent_socket_connect(bev, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
	bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);

	ev = event_new(base, STDIN_FILENO, EV_READ|EV_PERSIST, read_terminal, bev);
	event_add(ev, NULL);
	event_base_dispatch(base);

	event_free(ev);
	event_base_free(base);
	return;
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
