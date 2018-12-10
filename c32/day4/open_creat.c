


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 打开文件， 不存在就创建， 存在就提示错误信息
 *  测试执行： ./open_creat filename
 */
int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        /*  打开文件， 如果不存在就创建， 存在就出错返回 */

        int fd ;
        fd = open(argv[1],  O_RDWR|O_CREAT|O_EXCL, 0644);
        if (fd < 0){
                printf("Error for open %s:%s\n", argv[1],  strerror(errno));
                return 1;
        }

        /*  关闭 */

        close(fd);
        return 0;
}
