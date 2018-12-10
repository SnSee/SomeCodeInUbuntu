#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <event2/event.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 * function: the read end of libevent 
 */
void read_cb(evutil_socket_t fd, short what, void *arg){
	char buf[1024];
	int len_r = read(fd, buf, sizeof(buf));
	write(STDOUT_FILENO, buf, strlen(buf));
	return;
}
void func1(){
	unlink("eventfifo");	
	mkfifo("eventfifo", 0644);

	int fd = open("eventfifo", O_RDONLY | O_NONBLOCK);
	if(fd < 0){
		perror("open err");
		exit(-1);
	}
	struct event_base *base = event_base_new();
	struct event *ev = event_new(base, fd, EV_READ|EV_PERSIST, read_cb, NULL);
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
