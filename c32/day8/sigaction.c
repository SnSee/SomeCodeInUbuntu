

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

/**
 *  功能描述：  sigaction 的基本用法， 捕捉sigquit
 *  测试执行： ./sigaction
 */

void sig_handler(int sig)
{
        printf("哈哈，我捕捉到了信号\n");
        return ;
}

int main(int argc, char *argv[])
{

        /*  出事化 sigaction */
        /* struct sigaction { */
        /*         void     (*sa_handler)(int); */
        /*         void     (*sa_sigaction)(int, siginfo_t *, void *); */
        /*         sigset_t   sa_mask; */
        /*         int        sa_flags; */
        /*         void     (*sa_restorer)(void); */
        /* }; */

        struct sigaction sa;

        sa.sa_handler = sig_handler;
        sigemptyset(&sa.sa_mask);  /* 清空阻塞信号集，    */
        sa.sa_flags  =0;

        //     sa.sa_sigaction = NULL; /*  可有可无 */

        /* 捕捉信号， sigquit */
        if( sigaction(SIGQUIT,  &sa, NULL) < 0){
//      if( sigaction(SIGINT,  &sa, NULL) < 0){
                printf("Error for sigaction:%s\n", strerror(errno));
                return 1;
        }
                
        /*  子进程 */
        while(1){
                printf("：我还活着\n");
                sleep(1);
        }
        
        return 0;
}



