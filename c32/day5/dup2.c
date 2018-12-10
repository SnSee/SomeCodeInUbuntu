

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： dup2 完成在指定的文件描述符上，复制文件描述
 *  测试执行： ./dup2  file1 file2
 */
int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 3){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        /* 打开第一个文件 */
        int fd1 = open(argv[1], O_RDWR);
        printf("fd1=%d\n", fd1);


        /* 打开第二个文件 */
        int fd2 = open(argv[2], O_RDWR);

        printf("fd2=%d\n", fd2);

        /* 暂不做dup2，  */
        dup2(fd1, fd2);
                
        /*   对fd1， fd2， 各写一个字符A. */
        write(fd1, "A", 1);
        write(fd2, "A", 1);

        /* 关闭打开的文件 */

        close(fd1);
        close(fd2);
        
        return 0;
}
