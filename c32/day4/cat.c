
#include <stdio.h>
#include <string.h>

#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * 实现cat 一个文件到屏幕， 文件从命令行传入
 *
 *   测试执行： ./cat filename 
 */

int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        /* 1.  打开文件 */
       /*   int open(const char *pathname, int flags); */
       /* int open(const char *pathname, int flags, mode_t mode); */

        int fd = open(argv[1], O_RDONLY);
        if (fd < 0){
                /* 错误信息 */
                printf("Error for open :%s: %s\n", argv[1], strerror(errno));
                return 1;
        }

        /* 2: 读写文件， 循环 */
               /* ssize_t read(int fd, void *buf, size_t count); */
        char buf[1024];
        int len_r;

        len_r = read(fd, buf, sizeof(buf));
        if (len_r < 0){
                printf("Error for read: %s\n",  strerror(errno));
                /* return 0; */
        }

        /* 2.5: 写到屏幕上去 ， stdout, */
        /* ssize_t write(int fd, const void *buf, size_t count); */
        int len_w;

        len_w = write(1, buf,  len_r);
        if (len_w != len_r){
                printf("Error for write: %s\n", strerror(errno));
                /* return 0; */
        }
        
        /* 3： 关闭文件 */
        close(fd);
        return 0;
}
