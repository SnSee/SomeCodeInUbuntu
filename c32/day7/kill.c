

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
 *  功能描述：  kill基本用法，  对子进程发送信号， sigint
 *  测试执行： ./kill
 */
int main(int argc, char *argv[])
{

        /*  创建子进程 */

        pid_t pid = fork();

        if(pid == 0){
                /*  子进程 */
                while(1){
                        printf("子进程：我还活着\n");
                        sleep(1);
                }
                exit(1);
        }

        /* 父进程 */
        sleep(5);
        /*  发送sigint信号， 到子进程 */
        /*  在父进程中， fork返回子进程的pid */
        kill(pid, SIGINT);

        wait(NULL);             /*  回收子进程资源 */
        
        return 0;
}
