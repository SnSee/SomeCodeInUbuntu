


#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define N 1024

extern char * file_type_mode(const struct stat *st,  char *buf);
        
/**
 *  功能描述： 用stat函数获取文件的属性
 *  测试执行： ./stat  filename 
 */
int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        /* stat获取文件属性 */
       /* int stat(const char *pathname, struct stat *statbuf); */

        struct stat st;
        if (stat(argv[1],  &st) < 0){
                /* 获取文件属性出错 */
                printf("Error for stat %s:%s\n", argv[1], strerror(errno));
                return 1;
        }
        
        /* TODO：  显示文件属性 */

        char buf[N];
        /* 处理文件的类别和权限， 就是分析 st.st_mode  */
        printf("%s\t", file_type_mode(&st, buf));
        
        /* 显示文件长度 */
        printf(" %ld\t", st.st_size); /*  文件大小 */
        
        printf("%s\n", argv[1]); /* 文件名 */

        
        return 0;
}


char * file_type_mode(const struct stat *st,  char *buf)
{
        /*  把 st->st_mode  解析成"drwxr-xr--" , 填写到buf中，  */
        
        //        strcpy(buf, "drwxr--r--");    /*  Why? 为什么？ */

        int i = 0;
        /* 文件类别 */

        /* if (S_ISREG(st->st_mode)) */
        /*         buf[i++] = '-'; */
        /* else if (S_ISDIR(st->st_mode)) */
        /*         buf[i++] = 'd'; */
                 
        /* else */
        /*         buf[i++] ='x'; */

        switch(st->st_mode & S_IFMT){
        case S_IFREG:
                buf[i++] = '-';
                break;
        case S_IFDIR:
                buf[i++] = 'd';
                break;
                /* ...... */

        default:
                buf[i++] = '-';
        }

        /* 处理文件属主的权限 */
        /* buf[123] = rwx- */

        /* 文件的属主位置，读权限有无的问题 */
        if ((st->st_mode & S_IRUSR) != 0){
                buf[i++] = 'r';
        } else{
                buf[i++] = '-';
        }
        /*  文件属主，写权限 */
        if ((st->st_mode & S_IWUSR) != 0)
                buf[i++] = 'w';
        else
                buf[i++] = '-';

        /* 文件属主， 执行权限 */
        if (st->st_mode & S_IXUSR)
                buf[i++] = 'x';
        else
                buf[i++] = '-';

        /*  TODO: 你补齐： man 7 inode */
        
        buf[i] = '\0';
        return buf;
}
