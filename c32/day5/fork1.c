


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 进程的的创建， 基本用法
 *  测试执行：
 */
int main(int argc, char *argv[])
{

        /* 创建进程 */
        /* pid_t fork(void); */

        pid_t pid = fork();

        if (pid < 0){
                /*  出错 */
        } else if (pid == 0){
                /*  子进程 */
                /*  TODO： */
                printf("子进程\n");
                /* exec */
        } else {
                /* 父进程 */
                printf("父进程\n");        
        }

       
        return 0;
}
