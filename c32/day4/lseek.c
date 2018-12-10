

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 验证lseek接口， 读取文件，隔一个读一个，循环读
 *  测试执行：  ./lseek   filename
 */
int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        /* 1; 打开文件 */

        int fd = open(argv[1], O_RDWR);
        if (fd < 0){
                /* 打开失败 */
                printf("Error for open %s: %s\n", argv[1], strerror(errno));
                return 1;
        }

        /* 读取文件内容， 一次一个字节， 且隔一个读一个 */
        char ch[1];
        int len;

        do {
                len = read(fd, ch, 1);
                if (len == 0)
                        break;
                write(STDOUT_FILENO, ch, 1);
                lseek(fd, 1, SEEK_CUR); /*  移动读写位置，从当前的读写位置后移一个字节，  */
                
        }  while(len != 0);

        write(1, "\n", 1);
        /* 3: 关闭文件 */
        close(fd);
        
        return 0;
}
