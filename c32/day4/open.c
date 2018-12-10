#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

        /* argc : 命令行参数的个数 */
//        printf("argc: %d\n", argc);
        /* argv[], 对应命令行参数 */
//       printf("argv0:%s\t argv1:%s\n", argv[0], argv[1]);

        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }
        
       /*   int open(const char *pathname, int flags); */
       /* int open(const char *pathname, int flags, mode_t mode); */
        
        /* 打开文件 */

        int fd ;

        fd = open(argv[1], O_RDONLY);
        if (fd < 0){
                /* 打开文件失败， 输出一些错误信息 */
                perror("Error for open");
                return 1;
        }
        printf("fd = %d\n", fd);
        /* 关闭文件 */
        close(fd);
        
        return 0;
}
