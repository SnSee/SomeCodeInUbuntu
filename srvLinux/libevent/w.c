#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <event2/event.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

/*
 * function: write end of libevent test 
 */
void write_cb(evutil_socket_t fd, short what, void *arg){
	char buf[] = "hello";
	int len_w = write(fd, buf, strlen(buf) + 1);
	sleep(1);
	return;
}
void func1(){
	int fd = open("eventfifo", O_WRONLY|O_NONBLOCK);
	if(fd < 0)
		perror("open err");
	
	struct event_base *base = event_base_new();
	struct event *ev = event_new(base, fd, EV_WRITE|EV_PERSIST, write_cb, NULL);
	event_add(ev, NULL);
	event_base_dispatch(base);

	event_free(ev);
	event_base_free(base);
	close(fd);
	return;	
}

int main(int argc, char *argv[])
{
	func1();

	return 0;
}
