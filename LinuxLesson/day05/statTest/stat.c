#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define N 1024

//int stat(const char *pathname, struct stat *statbuf);

char *getFileMode(const struct stat *st, char *buf);

int main(int argc, char *argv[]){
	struct stat st;
	if(stat(argv[1],&st) < 0){
		printf("Error for stat %s:%s", argv[1], strerror(errno));
		return -1;
	}
	/*TODO: show the attributes of the file*/
	char buf[1024];
	/*show the mode */
	printf("mode: %s",getFileMode(&st, buf));

	return 0;
}
//the function that return fileMode
char *getFileMode(const struct stat *st, char *buf){

	//strcpy(buf, "string");
//	if ((sb.st_mode & S_IFMT) == S_IFREG) {
//               /* Handle regular file */
//      }
	int i = 0;
	if(S_IFREG == (st->st_mode & S_IFMT)){
		buf[i++] = '-';
	}
	else if(S_IFDIR == (st->st_mode & S_IFMT)){
		buf[i++] = 'd';
	}
	else{
		buf[i++] = '%';
	}

	buf[i] = '\0';
	return buf;
}
