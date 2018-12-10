
#include <stdio.h>

extern int add(int a, int b );
extern int sub(int , int);

int main(int argc, char *argv[])
{
        int a = 5;
        int b = 4;
        printf("sub: %d\n",sub(a,b));
        printf("add: %d\n", add(a, b));
        return 0;
}
