
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 非阻塞读取终端
 *  测试执行： ./no_block
 */
#define N 1024
int main(int argc, char *argv[])
{

        /* 1: 打开终端 */
        int fd;
        /* 以非阻塞的读写方式， 打开终端/dev/tty */
        fd = open("/dev/tty", O_RDWR|O_NONBLOCK);
        //        fd = open("/dev/tty", O_RDWR);
        if (fd < 0){
                printf("Error for open %s:%s\n", "/dev/tty", strerror(errno));
                return 1;
        }

        char buf[N];
        int len;

        while(1){
                /* 读终端，  */
                len = read(fd, buf, sizeof(buf));
                if (len < 0){
                        perror("Error for read");
                        sleep(1);
                        continue;
                }
           
                /* 把读到的数据 ， 写到标准输出 */
                write(1,  buf, len);
                write(1, "\n", 1);
                break;
        }

        /* 关闭终端 */
        close(fd);
        return 0;
}
