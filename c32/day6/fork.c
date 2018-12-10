
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define N 10
/**
 *  功能描述： 创建N个子进程， 在每个子进程中输出自己的序号
 *  测试执行： ./fork
 */
int main(int argc, char *argv[])
{
        

        pid_t pid;


        int i;

        for (i = 0; i < N; i++){
                pid = fork();
                if(pid == 0)
                        break;
        }
        
        if(i == 0){
                /* 第一个子进程 */
                printf("第一个子进程\n");
                exit(0);
        }

        /* 如何处理2， 3个 */
        if (i >0  && i < N){
                printf("第%d个子进程\n", i+1);
                exit(0);
        }

        if (i == N){
                /* 父进程 */
                printf("父进程\n");
                
        }
        
        return 0;
}
