#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	printf("with change line\n");
	printf("no change line");
	printf("no change line2");
	
	sleep(5);
	int a;
	scanf("%d", &a);
	return 0;
}
