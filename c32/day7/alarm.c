

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

/**
 *  功能描述：  alarm基本用法，  延时20秒终止当前进程
 *  测试执行： ./kill
 */
int main(int argc, char *argv[])
{


        /*  注册一个闹钟 */
        alarm(5);
        
        /*  子进程 */
        while(1){
                printf("：我还活着\n");
                sleep(1);
        }
        
        return 0;
}
