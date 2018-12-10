

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述：
 *  测试执行：
 */
int main(int argc, char *argv[])
{


        char buf[1024];
        int len = read(0,  buf, 1024);

        write(1, buf, len);
        write(1, "\n", 1);

        printf("当前终端; %s\n", ttyname(0));
        
        return 0;
}
