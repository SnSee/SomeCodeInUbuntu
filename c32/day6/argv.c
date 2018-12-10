

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

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        if (strcmp(argv[0], "cat") == 0){
                /* 做一些操作。。 */
                execlp("cat", "aaa", "./argv.", NULL);
        } else if(strcmp(argv[0], "ls") == 0){
                execlp("ls", "ls", NULL)   ;
        } else{
                ;
                /* Nothing to do */
        }
                       
        
        return 0;
}
