


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 阻塞式读终端， （stdin）
 *  测试执行： ./block
 */

#define N 1024

int main(int argc, char *argv[])
{

        char buf[N];

        /*读取标准输入， 0， */
        int len = read(STDIN_FILENO, buf, sizeof(buf));
        if (len < 0){
                /* TODO: 容错处理 */
                return 1;
        }

        write(1,  "Hello\t",  6);
        /*  写读到的数据到标准输出 */
        write(STDOUT_FILENO, buf, len);
        write(1, "\n", 1);
        
        return 0;
}
