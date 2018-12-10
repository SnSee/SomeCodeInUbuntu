#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <event2/event.h>
#include <event2/bufferevent.h>
#include <event2/listener.h>
#include <sys/socket.h>
#include <string.h>

#define SRV_PORT 9876 
/*
 * function: create a server with bufferevent of livevent 
 */
//the read callback function
void read_cb(struct bufferevent *bev, void *arg){
	char buf[1024];
	int len_r = bufferevent_read(bev, buf, sizeof(buf));
	printf("len_r = %d\n", len_r);
	printf("client:%s", buf);
	bzero(buf, sizeof(buf));
	sprintf(buf, "%d bytes received\n", len_r);
	printf("%s", buf);
	//int len_w = bufferevent_write(bev, buf, strlen(buf) + 1);
	sleep(1);
	return;
}

//the write callback function
void write_cb(struct bufferevent *bev, void *arg){
	//printf("message send to client\n");
	return;
}

//the event callback function
void event_cb(struct bufferevent *bev, short what, void *arg){
	if(what & BEV_EVENT_EOF){
		printf("client closed\n");
	}
	else if(what & BEV_EVENT_ERROR){
		printf("unknown error\n");
	}
	else if(what & BEV_EVENT_CONNECTED){
		return;
	}
	bufferevent_free(bev);
	printf("struct bufferevent object released\n");
	return;
}
//the callback function which should be called when a client connects with the server
void listener_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *addr, int len, void *ptr){
	//get the argument passed by pointer ptr
	struct event_base *base = (struct event_base*)ptr;

	//add new bufferevent
	struct bufferevent *bev;
	bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE);
	//add 3 callback functions	
	bufferevent_setcb(bev, read_cb, write_cb, event_cb, NULL);
	bufferevent_enable(bev, EV_READ);	
}
void func1(){
	//server ip and port number
	struct sockaddr_in srvaddr = {
		AF_INET, htons(SRV_PORT), htonl(INADDR_ANY)
	};	
	//vabiables
	struct event_base *base;
	struct evconnlistener *listener;

	base = event_base_new();
	listener = evconnlistener_new_bind(base, listener_cb, (void*)base, LEV_OPT_CLOSE_ON_FREE|LEV_OPT_REUSEABLE, -1, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
	event_base_dispatch(base);

	evconnlistener_free(listener);
	event_base_free(base);
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
