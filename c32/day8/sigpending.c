

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <signal.h>

/**
 *  功能描述： sigpending 获取未决信号集
 *  测试执行： ./sigpending
 */
int main(int argc, char *argv[])
{

        /*  初始化阻塞信号集 */

        sigset_t set;           /*  定义信号集 */

        sigemptyset(&set);      /*  对信号集清0 */

        sigaddset(&set, SIGINT); /*  添加sigint信号到信号集， C-c */
        /* .... */
         
        if (sigismember(&set, SIGINT)){
                printf("有sigint信号\n");
        }

        /* sigprocmask 设置当前进程的阻塞信号集 */
        /* int sigprocmask(int how, const sigset_t *set, sigset_t *oldset); */
        sigset_t oldset;        /* 用于保存上一次阻塞信号集的值 */
        sigprocmask(SIG_BLOCK, &set, &oldset );


        /*  获取未决信号集 */

        sigset_t pset;
        int i  = 0;
        while(i++ < 5){
                sigpending(&pset);

                if (sigismember(&pset, SIGINT)){
                        printf("未决信号集中有sigint信号\n");
                }
                sleep(1) ;
        }


        /* 恢复之前的阻塞信号集 */
        sigprocmask(SIG_SETMASK, &oldset, NULL);

        
        return 0;
}
