
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 *  功能描述： wait的基本用法, 注意，子进程后执行， 父进程wait ，处于阻塞状态
 *  测试执行： ./wait
 */
int main(int argc, char *argv[])
{

        pid_t pid;

        pid = fork();

        if (pid == 0){
                /* 子进程 */
                sleep(2);
//                *(int *)0=4;
                printf("子进程： ppid:%d\tpid:%d\n", getppid(), getpid()); 
                exit(1);
        }

        /* 父进程 */

        int st;
        pid_t pid_c;
        pid_c = wait(&st);

        if (WIFEXITED(st))
                printf("子进程正常退出\n");
        
        printf("父进程： ppid:%d\tpid:%d\t 子进程%d\n", getppid(), getpid(), pid_c);
        
        return 0;
}
