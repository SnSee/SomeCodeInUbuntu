
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>

pthread_t ntid = 0;                 /*  线程共享全局变量 */
/*  线程体函数 */

void * pthread_routine(void *v)
{

        ntid = pthread_self() ;
        
        printf("第%d个线程: ntid=%p\n", (int) v, (void *)ntid );
        return NULL;
}


/**
 *  功能描述： pthread_equal 基本用法
 *  测试执行：./pthread_equal
 */
int main(int argc, char *argv[])
{

        /*  创建线程 */
        
        /* int pthread_create(pthread_t *thread, const pthread_attr_t *attr, */
        /*                    void *(*start_routine) (void *), void *arg); */

        pthread_t tid;
        pthread_create(&tid,  NULL,  pthread_routine, (void *) 1 );

        printf("主线程： tid=%p\n", (void *)tid);


        /*  判断， 线程id 是否相同 */

        if (pthread_equal(tid,  ntid) !=0){
                printf("线程ID相同\n");
        }
        
        /*  回收线程线程资源 */

        void * p;
        pthread_join(tid, &p);  /*  回收资源，  */

        printf("主线程\n");
        return 0;
}
