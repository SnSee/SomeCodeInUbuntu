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
void write_cb(evutil_socket_t fd, short what, void *arg){
	char buf[1024];
	int len_r;
	len_r = read(STDIN_FILENO, buf, sizeof(buf));
	if(len_r < 0){
		perror("read err");
	}
	write(fd, buf, strlen(buf) + 1);
	return;
}
void func1(){
	int fd;
	fd = open("fifo2", O_WRONLY|O_NONBLOCK);
	if(fd < 0){
		printf("no fifo2, create it\n");
		mkfifo("fifo2", 0644);
		open("fifo2", O_WRONLY|O_NONBLOCK);
	}	

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
