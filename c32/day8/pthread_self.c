
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
        printf("第%d个线程: tid=%p\n", (int) v,   (void *)pthread_self() );
        return NULL;
        /* pthread_exit(NULL);  === return NULL */
}


/**
 *  功能描述： pthread_self 基本用法, 获取线程id
 *  测试执行：./pthread_create
 */
int main(int argc, char *argv[])
{

        /*  创建线程 */
        
        /* int pthread_create(pthread_t *thread, const pthread_attr_t *attr, */
        /*                    void *(*start_routine) (void *), void *arg); */

        pthread_t tid;
        pthread_create(&tid,  NULL,  pthread_routine, (void *) 1 );

        printf("第1个线程id： %p\n", (void *) tid);
        
        pthread_create(&tid,  NULL,  pthread_routine, (void *) 2 );
        printf("第2个线程id： %p\n", (void *) tid);
        pthread_create(&tid,  NULL,  pthread_routine, (void *) 3 );
        printf("第3个线程id： %p\n", (void *) tid);
        
        
        sleep(2);
        printf("主线程\n");
        return 0;
}
