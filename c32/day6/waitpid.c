
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
 *  功能描述： waitpid的典型用法,
 *  测试执行： ./waitpid
 */
int main(int argc, char *argv[])
{

        pid_t pid;

        /*  创建子进程 */
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
        /* pid_c = waitpid( -1, &st, 0);   和 wait 相同 */

        pid_c = waitpid( -1, &st,  WNOHANG); /* 记住， 在信号处理函数中的典型用法  */

        if (pid_c <= 0){
                printf("子进程回收失败\n");
        }
        /* if (WIFEXITED(st)) */
        /*         printf("子进程正常退出\n"); */
        
        printf("父进程： ppid:%d\tpid:%d\t 子进程%d\n", getppid(), getpid(), pid_c);
        
        return 0;
}
