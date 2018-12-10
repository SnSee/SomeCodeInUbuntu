


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： execlp 函数应用示例： fork子进程， 在子进程中执行 cat execlp.c , 父进程打印信息
 *  测试执行： ./execl-1
 */
int main(int argc, char *argv[])
{

        /* int execl(const char *path, const char *arg, ... */
        /*                /\* (char  *) NULL *\/); */


        /* 创建一个子进程 */
        pid_t pid = fork();


        if (pid == 0){
                /* 子进程 */

                execlp("cat", "cat",  "./execlp.c", NULL);
                printf("Error for execlp: %s\n", strerror(errno));
                
                exit(0);
        }

        /*  */
        sleep(1);
        printf("我是父进程\n");
        
        return 0;
}
