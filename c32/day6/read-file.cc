


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 用fifo实现无血缘关系的通信， 读fifo
 *  测试执行： ./read-file  fifo-file
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

        fd = open(argv[1], O_RDONLY);

        char buf[1024];
        int len;

        len = read(fd, buf, sizeof(buf));
        buf[len] = '\0';        /*  read返回值的典型用法 */
        buf[len] = '\0';        /*  read返回值的典型用法 */

        printf("buf:%s\n", buf);"")'']
        printf("buf:%s\n", buf);

        close(fd);
        return 0;
}
