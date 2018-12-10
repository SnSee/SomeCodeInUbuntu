

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： pipe基本用法，在当前进程中，探讨pipe读写
 *  测试执行： ./pipe-1
 */
int main(int argc, char *argv[])
{

        int fd[2];              /*  先声明数组 */

        /* 创建管道 */
        if (pipe(fd) < 0){
                printf("Error foir pipe : %s\n", strerror(errno));
                return 1;
        }

        /*  管道创建成功的， 我们就拥有l额两个文件描述符 */

        /*  先往管道中写点内容 */
        write(fd[1], "A", 1);

        /* 从管道中读取内容 */
        char ch = 'a';

        read(fd[0], &ch, 1);
        
        printf("%c\n", ch);
        
        return 0;
}
