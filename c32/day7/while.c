

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： 死循环
 *  测试执行：./while
 */
int main(int argc, char *argv[])
{

        /* 死循环 */

        while(1);
        return 0;
}
