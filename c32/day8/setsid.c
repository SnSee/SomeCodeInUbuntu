

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/** 
 *  功能描述：  创建守护进程， 
 *  测试执行： ./setsid
 */
int main(int argc, char *argv[])
{

        /*  1. 创建会话 */
        /*  创建子进程， 父进程退出， 子进程执行setsid */

        pid_t  pid = fork();

        if(pid > 0)             /* 父进程退出 */
                exit(1);

        /*  在子进程中创建会话 */
        setsid();

        /* 2. 更改为根路径 */
        chdir("/");


        /*  3. 关闭标准输入，输出。。。 */
        close(0);
        close(1);
        close(2);


        /* 。。。。 */

        /* 处理你该处理的事物 */
        while(1){
                ;               /*  该干啥干啥去 */
                sleep(1);
        }
        
        return 0;
}
