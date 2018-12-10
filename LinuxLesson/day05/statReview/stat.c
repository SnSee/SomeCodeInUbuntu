#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#define N 1024

char *getFileMode(const struct stat *st, char *buf);
int main(int argc, char *argv[]){
	if(argc != 2){
		printf("need 2 arguments");
		return -1;
	}
	struct stat st;
	int ret = stat(argv[1], &st);
	char buf[N];
	//call the method which return the file type and mode 
	getFileMode(&st, buf);
	//file type and mode
	printf("%s ", buf);
	//number of hard links
	printf("%ld ", st.st_nlink);
	//owner
	printf("%d ", st.st_uid);
	//group
	printf("%d ", st.st_gid);
	//size
	printf("%ld ", st.st_size);
	//change line
	printf("\n");
	return 0;
}
//the method which return the file type and mode 
char *getFileMode(const struct stat *st, char *buf){
	int i = 0;
	//file type
	if((st->st_mode & S_IFMT) == S_IFREG){
		buf[i++] = '-';
	}	
	else if(S_ISDIR(st->st_mode)){
		buf[i++] = 'd';
	}
	else if(S_ISCHR(st->st_mode)){
		buf[i++] = 'c';
	}
	else if(S_ISBLK(st->st_mode)){
		buf[i++] = 'b';
	}
	else if(S_ISFIFO(st->st_mode)){
		buf[i++] = 'p';
	}
	else if(S_ISLNK(st->st_mode)){
		buf[i++] = 'l';
	}
	else if(S_ISSOCK(st->st_mode)){
		buf[i++] = 's';
	}
	else{
		buf[i++] = '-';
	}
	
//owner's mode
	//read mode
	if(st->st_mode & S_IRUSR)
		buf[i++] = 'r';
	else 
		buf[i++] = '-';
	//write mode
	if(st->st_mode & S_IWUSR)
		buf[i++] = 'w';
	else
		buf[i++] = '-';
	//execute mode
	if(st->st_mode & S_IXUSR)
		buf[i++] = 'x';
	else
		buf[i++] = '-';
//group's mode
	//read mode
	if(st->st_mode & S_IRGRP)
		buf[i++] = 'r';
	else 
		buf[i++] = '-';
	//write mode
	if((st->st_mode & S_IWGRP) != 0)
		buf[i++] = 'w';
	else
		buf[i++] = '-';
	//execute mode
	if(st->st_mode & S_IXGRP)
		buf[i++] = 'x';
	else
		buf[i++] = '-';
//other's mode
	//read mode
	if(st->st_mode & S_IROTH)
		buf[i++] = 'r';
	else 
		buf[i++] = '-';
	//write mode
	if(st->st_mode & S_IWOTH)
		buf[i++] = 'w';
	else
		buf[i++] = '-';
	//execute mode
	if(st->st_mode & S_IXOTH)
		buf[i++] = 'x';
	else
		buf[i++] = '-';
//set the end of the string to '\0'
	buf[i] = '\0';
	return buf;
}
