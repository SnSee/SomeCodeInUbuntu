


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 复制一个文件描述符
 *  测试执行：
 */
int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        int fd = open(argv[1], O_RDWR);


        int fd_n = dup(fd);

//        lseek(fd, 0, SEEK_END );

        write(fd, "a", 1);

        write(fd_n, "b", 1);


       /* int dup(int oldfd); */
       /* int dup2(int oldfd, int newfd); */

        /* close */
        close(fd);
        return 0;
}
