
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述：
 *  测试执行：
 */
int main(int argc, char *argv[])
{

        pid_t pid;

        printf("父进程： ppid:%d\tpid:%d\n", getppid(), getpid());
        
        pid = fork();

        if (pid == 0){
                /* 子进程 */
//                sleep(2);                
                printf("子进程： ppid:%d\tpid:%d\n", getppid(), getpid()); 
                exit(0);
        }

        /* 父进程 */

        printf("父进程\n");
        printf("父进程： ppid:%d\tpid:%d\n", getppid(), getpid());


        while(1);
        
        return 0;
}
