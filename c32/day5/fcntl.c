


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


#define  N 1024
/**
 *  功能描述： 用fcntl 变更终端的阻塞和非阻塞属性
 *  测试执行：  ./fcntl 
 */
int main(int argc, char *argv[])
{

        /*  读终端， */
        char buf[N];
        int len; 
        int i = 0;              /*  用于控制读的次数，读两次 */


        /*  变更终端为非阻塞方式 ， fcntl*/

        /* int fcntl(int fd, int cmd, ... /\* arg *\/ ); */

        int flags;

        /*  1: STDOUT_FILENO */
        /*  XXX： 变更文件描述符的标志，三部曲， 需要记住 */
        flags = fcntl(1, F_GETFL); /*  获取当前文件的 flags （open指定那个flags） */

        flags = flags | O_NONBLOCK;

        fcntl(1, F_SETFL, flags); /* 设置指定fd文件的flags */
        

        while(i < 2){
                /* 读终端操作 */

                len =read(1,  buf, sizeof(buf));

                if (len < 0){
                        printf("read error : %s\n", strerror(errno));
                        sleep(1);
                        continue;
                }
                i++;
                write(1, buf, len);
        }

        /* 写终端 */
        
        return 0;
}
