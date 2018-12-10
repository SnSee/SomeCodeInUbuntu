

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
 *  功能描述：  signal 的基本用法， 
 *  测试执行： ./signal
 */

void sig_handler(int sig)
{
        printf("哈哈，我捕捉到了信号\n");
        return ;
}

int main(int argc, char *argv[])
{

        /*  注册， alarm信号的捕捉函数 */
        /* sighandler_t signal(int signum, sighandler_t handler); */
        signal(SIGALRM, sig_handler);

        /*  注册一个闹钟 */
        alarm(5);
        
        /*  子进程 */
        while(1){
                printf("：我还活着\n");
                sleep(1);
        }
        
        return 0;
}
