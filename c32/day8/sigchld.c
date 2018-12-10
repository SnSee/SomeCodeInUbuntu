


#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/wait.h>
#include <signal.h>

void sig_child(int sig)
{
        printf("子进程死了\n");
        /*  回收资源 */
        /* wait(NULL); */             /*  不好 */
        while (waitpid(-1, NULL,  WNOHANG) > 0);
        return ;
}


/**
 *  功能描述： 用sigchld 信号回收子进程资源
 *  测试执行： ./sigchld
 */

int main(int argc, char *argv[])
{
        /*  捕捉，sigchld信号 */

       signal(SIGCHLD,  sig_child);


        pid_t pid = fork();

        if ( pid == 0){
                /* 子进程 */

                printf("我被创建过\n");
                exit(0);
        }

        /* 父进程  */

        /* wait */

        while(1){
                sleep(1)      ;
                printf("父进程还活着: cpid=%d\n", pid);
        }
        
        return 0;
}
