
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>

/*  线程体函数 */

void * pthread_routine(void *v)
{

        int i ;

        for (i = 0; i < 15; i ++){
                sleep(1);
                printf("第%d个线程, 还活着\n", (int) v );                
        }

        return NULL;
}


/**
 *  功能描述： pthread_detack， 设置线程分离属性， 用于线程自动回收 基本用法
 *  测试执行：./pthread_equal
 */
int main(int argc, char *argv[])
{

        /*  创建线程 */
        
        /* int pthread_create(pthread_t *thread, const pthread_attr_t *attr, */
        /*                    void *(*start_routine) (void *), void *arg); */

        pthread_t tid;
        pthread_create(&tid,  NULL,  pthread_routine, (void *) 1 );


        /*  设定线程的分离属性 */
        pthread_detach(tid);

        
        printf("主线程： pid=%d\n", getpid());

        
        /*  不回收线程线程资源 */


        
        /* void * p; */
        /* pthread_join(tid, &p);  /\*  回收资源，  *\/ */


        while(1){
                sleep(1);
                printf("主线程\n");
        }
        return 0;
}
