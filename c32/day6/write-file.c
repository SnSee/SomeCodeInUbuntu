


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 用fifo实现无血缘关系的通信， 写入fifo
 *  测试执行： ./write-file  fifo-file
 */
int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        /* 打开fifo'文件' */
        int fd;

        fd = open(argv[1], O_WRONLY);

        char *p = "Hello World";

        write(fd, p, strlen(p));

        close(fd);
        return 0;
}
