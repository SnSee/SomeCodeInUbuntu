

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>

#define NLOOP 5000
#define N 2

int counter = 0;                /*  这个就是多线程的共享的资源 */

void * thread(void *arg)
{
        int i;

        /*  线程的处理，  */
        
        for (i = 0; i < NLOOP; i++){
                
                usleep(1000);
                
                int val = counter;
                
                printf("第%d线程: counter=%d\n", (int )arg, val +1);
                counter = val + 1; 

        }
        
        return NULL;
}

/**
 *  功能描述： 创建两个线程，  每个线程执行自己的任务
 *  测试执行：./pthread_create2
 */
int main(int argc, char *argv[])
{


        /* nt pthread_create(pthread_t *thread, const pthread_attr_t *attr,
           void *(*start_routine) (void *), void *arg);

        */

        pthread_t tid[N];

        /* 创建N线程 */
        
        int i;
        for (i = 0; i < N; i++)
                pthread_create(&tid[i], NULL, thread, (void *)i);


        /*  回收资源 */
        void *p; 
        for(i = 0; i <N; i++)
                pthread_join(tid[i], &p);

        /* 主线程打印 counter*/

        printf("counter:%d\n", counter);
        
        return 0;
}
