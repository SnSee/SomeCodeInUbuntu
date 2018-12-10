

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
 *  功能描述： pipe基本用法2， 实现从子到父进程的信息传递
 *  测试执行： ./pipe-2
 */
int main(int argc, char *argv[])
{

        int fd[2];              /*  先声明数组 */

        /* 创建管道 */
        if (pipe(fd) < 0){
                printf("Error foir pipe : %s\n", strerror(errno));
                return 1;
        }

        /* 创建父进程 */

        pid_t pid = fork();

        if (pid ==0) {
                /*  子进程,  写入信息，  */
                sleep(3);
                /* 关闭读端 */
                close(fd[0]);
                /* 写管道 */
                write(fd[1], "A", 1);

                /* 关闭写端 */
                close(fd[1]);
                exit(0);
        }

        /*  父进程 读取信息*/
        /* 1： 读取管道 */

        /* 关闭写端 */
        close(fd[1]);

        char ch = 'a';
        read(fd[0], &ch, 1);

        printf("%c\n", ch);

        close(fd[0]);           /* 关闭读端 */
        
        /* 2： 回收子进程资源 */
        wait(NULL);
        return 0;
}
