
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述：  全局变量， 在父子进程是否共享
 *  测试执行：
 */

int a = 5;

int main(int argc, char *argv[])
{

        pid_t pid = fork();

        if (pid == 0){
                /* 子进程 */

                a += 10;
                printf("子进程： a:%d\n", a);

                exit(0);
        }

        /* 父进程 */
        a += 20;
        printf("父进程 a: %d\n", a);
        return 0;
}
