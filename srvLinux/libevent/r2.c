#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <event.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*
 * function: an instance of libevent library
 */
void read_cb(evutil_socket_t fd, short what, void *arg){
	char buf[1024];
	int len_r;
	len_r = read(fd, buf, sizeof(buf));
	if(len_r < 0){
		perror("read err");
	}
	printf("%s", buf);
	return;
}
void func1(){
	int fd;
	if(0 > (fd = open("fifo2", O_RDONLY|O_NONBLOCK))){
		printf("no fifo2, create it\n");
		mkfifo("fifo2", 0644);
		open("fifo2", O_RDONLY|O_NONBLOCK);
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
