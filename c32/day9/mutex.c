

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>

#define NLOOP 5000
#define N 10

int counter = 0;                /*  这个就是多线程的共享的资源 */

pthread_mutex_t  mutex = PTHREAD_MUTEX_INITIALIZER;

void * thread(void *arg)
{

        int i;

        /*  线程的处理，  */
        
        for (i = 0; i < NLOOP; i++){
                
//                usleep(1000);

                /*  对临界区加锁 */
                pthread_mutex_lock(&mutex); /*  加锁 */
                
                
                int val = counter;
                
                printf("第%d线程: counter=%d\n", (int )arg, val +1);
                counter = val + 1;
                pthread_mutex_unlock(&mutex); /*  mutex释放， 一定 */
        }
        
        return NULL;
}


void * thread2(void *arg)
{

        int i;

        /*  线程的处理，  */
        
        for (i = 0; i < NLOOP; i++){
                
//                usleep(1000);

                /*  对临界区加锁 */
 //             pthread_mutex_lock(&mutex); /*  加锁 */
                
                int val = counter;
                
                printf("第%d线程: counter=%d\n", (int )arg, val +1);
                counter = val + 1;
                //               pthread_mutex_unlock(&mutex); /*  mutex释放， 一定 */
        }
        
        return NULL;
}


/**
 *  功能描述： 创建两个线程，  每个线程执行自己的任务
 *  测试执行：./pthread_create2
 */
int main(int argc, char *argv[])
{

        int a = 0;
        a++;

        /* nt pthread_create(pthread_t *thread, const pthread_attr_t *attr,
           void *(*start_routine) (void *), void *arg);

        */

        pthread_t tid[N];

        /* 创建N线程 */
        
        int i;
        for (i = 0; i < N; i++)
                pthread_create(&tid[i], NULL, thread, (void *)i);
        /* pthread_create(&tid[1], NULL, thread2, (void *)1); */


        /*  回收资源 */
        void *p; 
        for(i = 0; i <N; i++)
                pthread_join(tid[i], &p);

        /* 主线程打印 counter*/

        printf("counter:%d\n", counter);
        
        return 0;
}
