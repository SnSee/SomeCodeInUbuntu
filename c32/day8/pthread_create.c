
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
        printf("第%d个线程\n", (int) v );
        return NULL;
}


/**
 *  功能描述： pthread_create 基本用法
 *  测试执行：./pthread_create
 */
int main(int argc, char *argv[])
{

        /*  创建线程 */
        
        /* int pthread_create(pthread_t *thread, const pthread_attr_t *attr, */
        /*                    void *(*start_routine) (void *), void *arg); */

        pthread_t tid;
        pthread_create(&tid,  NULL,  pthread_routine, (void *) 1 );
        pthread_create(&tid,  NULL,  pthread_routine, (void *) 2 );
        pthread_create(&tid,  NULL,  pthread_routine, (void *) 3 );
        

        
        
        sleep(2);
        printf("主线程\n");
        return 0;
}
