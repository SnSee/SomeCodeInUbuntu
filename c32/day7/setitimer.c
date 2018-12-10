

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

/**
 *  功能描述：  setitimer的基本用法
 *  测试执行： ./setitimer
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


        /*  int setitimer(int which, const struct itimerval *new_value,
            struct itimerval *old_value) */

        struct itimerval it;

        /* 处理延时 */
        it.it_value.tv_sec = 6; /*  6s */
        it.it_value.tv_usec = 0;

        /* 处理时间间隔 */
        it.it_interval.tv_sec = 1; /*  1s */
        it.it_interval.tv_usec = 1000000-1; /*  usec 取值范围， 0 ～ 999999 */

        setitimer(ITIMER_REAL, &it,  NULL);
        
        /*  子进程 */
        while(1){
//                printf("：我还活着\n");
                //              sleep(1);
        }
        
        return 0;
}
