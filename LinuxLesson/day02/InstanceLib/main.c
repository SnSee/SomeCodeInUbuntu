#include <stdio.h>
extern int add(int a, int b);
extern int sub(int a, int b);
int main(int argc, char *argv[]){
	int a = 5;
	int b = 4;
	printf("add: %d\n",add(a,b));
	printf("sub: %d\n",sub(a,b));
	return 0;
}
