

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 验证lseek接口， offset的取值
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

        /* 读取文件倒数第一个字符 */
        char ch='x';
//        int len;

        lseek(fd, -2, SEEK_END); /*  倒数第一个是换行， */
        read(fd, &ch, 1);
        write(1, &ch, 1);

        write(1, "\n", 1);
        /* 3: 关闭文件 */
        close(fd);
        
        return 0;
}
