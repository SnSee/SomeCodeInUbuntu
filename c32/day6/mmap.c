


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

/**
 *  功能描述： mmap 映射文件到内存， 打印输出
 *  测试执行：./mmap filename
 */
int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }


        /* 打开文件 */

        int fd = open(argv[1],  O_RDWR);

        char *p;

        int length = lseek(fd, 0, SEEK_END);
        
        /* 映射文件到内存 */
        /* void *mmap(void *addr, size_t length, int prot, int flags, */
        /*            int fd, off_t offset); */

        p = mmap(NULL,  length,  PROT_READ, MAP_PRIVATE, fd, 0);

        if (p == MAP_FAILED){
                /*  映射失败 */
                printf("error for mmap: %s\n", strerror(errno));
                goto err2;
        }

        /* 把文件的内容输出到屏幕 */

        if (write(1, p, length) < 0){
                /* 写入失败，  */
                printf("Error for write:%s\n", strerror(errno));
                goto err1;
        };

        /* ..... malloc/ free */
        
        /*  解除文件映射 */
        /* int munmap(void *addr, size_t length); */
err1:
        munmap(p, length);
        
err2:
        /* 关闭文件 */
        close(fd);
        return 0;
}
