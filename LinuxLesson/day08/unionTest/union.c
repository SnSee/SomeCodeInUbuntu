#include <stdio.h>
//test whether you can visit variable in anonymous union
//the answer is yes
struct te{
	union{
		int a;
		int b;
	};
};

int main(int argc, char *argv[]){
//	if(argc != 2){
//		printf("need 2 arguments\n");
//		return -1;
//	}
	struct te t;
	t.a = 10;
 	t.b = 20;
	printf("t.a = %d\n", t.a);
	return 0;
}
