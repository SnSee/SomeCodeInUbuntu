

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//#include <sys/types.h>
#include <dirent.h>


/**
 *  功能描述： 遍历指定的目录文件， 
 *  测试执行： ./opendir  dir
 */
int main(int argc, char *argv[])
{

        /*  判断命令行参数 */
        if (argc < 2){
                printf("Please input : %s <filename>\n", argv[0]);
                return 0;
        }

        /* 1： 打开目录 */
        /* DIR *opendir(const char *name); */

        DIR * dp = opendir(argv[1]);
        if ( dp == NULL){
                /* 打开目录文件出错 */

                printf("Error for  opendir %s : %s\n", argv[1], strerror(errno));
                return 1;
        }
        

        /* 2： 读取目录文件的内容 */
        /* struct dirent *readdir(DIR *dirp); */
        
        struct dirent * dir;
        
        while((dir= readdir(dp)) != NULL){
                /* 2.5 解析结构体， struct dirent */

                /* inode编号 */
                printf("%ld\t", dir->d_ino);

                /*  TODO:  文件类型*/
                if (dir->d_type == DT_REG) /* 普通文件 */
                        printf("普通文件\t");
                else if (dir->d_type == DT_DIR) /*  目录文件 */
                        printf("目录文件\t");
                /* 。。。。。 */
                else
                        printf("兄弟我不知到类型的文件\t");

                /*  文件名 */
                printf("%s", dir->d_name);
                /* 每一行， 换行 */
                printf("\n");
        }


         
        /* 3： 关闭目录 */
        closedir(dp);
        
        return 0;
}
