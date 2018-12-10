

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
/**
 *  功能描述： 用mmap实现父子进程的通信
 *  测试执行：./mmap-pc  file
 */
int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        /* 打开文件 */
        int fd = open(argv[1], O_RDWR|O_CREAT|O_TRUNC,  0644 );

        /*  把文件拓长 到100个字节 */
#define N 100
        lseek(fd, N-1, SEEK_SET);
        write(fd, " ", 1);

        /*  创建映射 */
        char *p = mmap(NULL, N,  PROT_READ|PROT_WRITE,  MAP_SHARED, fd, 0);
        if (p == MAP_FAILED){
                /*  映射失败 */
                printf("error for mmap:%s\n", strerror(errno));
                goto err2;
        }
        *p = 'a';               /*  这个值，将被父子进程都可以得到。 */
        /*  fork 子进程 */

        pid_t pid = fork();

        if (pid ==0){
                /* 子进程 */

                /* 子进程写 */
                *p='A';
                /*  strcpy(p, "Hello World"); */
                /* 子进程退出， 可以不考虑解除映射和关闭文件 */
                munmap(p, N);
                close(fd);
                exit(0);
        }

        /*  父进程 */
        sleep(1);
        printf("父进程：*p=%c\n", *p);
err1:
        /* 解除映射 */
        munmap(p, N);

err2:        
        /* 关闭文件 */
        close(fd);
        return 0;
}
