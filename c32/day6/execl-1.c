


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/**
 *  功能描述： execl 函数应用示例： 用指定程序的代码和数据替换当前进程的代码和数据
 *  测试执行： ./execl-1
 */
int main(int argc, char *argv[])
{

        /* int execl(const char *path, const char *arg, ... */
        /*                /\* (char  *) NULL *\/); */


        execl("/bin/ls", "ls", "-l", NULL);
        
        printf("Hello World\n");
        
        return 0;
}
