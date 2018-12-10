

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>

#define NLOOP 1000
#define N 10

int counter = 0;                /*  这个就是多线程的共享的资源 */

/*  初始化读写锁 */

pthread_rwlock_t rwlock;

void * thread_1(void *arg)      /*  写操作 */
{
        /* 加锁 */
        int i;

        /*  线程的处理，  */
        
        for (i = 0; i < NLOOP; i++){
                
//                usleep(1000);

                /*  对临界区加锁, 加写锁 */
                pthread_rwlock_wrlock(&rwlock); /*  加锁 */
                
                
                int val = counter;
                
                //              printf("第%d线程: counter=%d\n", (int )arg, val +1);
                counter = val + 1;

                /*  解锁 */
                pthread_rwlock_unlock(&rwlock); /*  释放写锁rwlock， 一定 */
        }

        /*  释放锁 */
        return NULL;
}


void * thread_2(void *arg)      /*  读线程 */
{

        int i;

        /*  线程的处理，  */
        
        for (i = 0; i < NLOOP; i++){
                
//                usleep(1000);

                /*  对临界区加读锁 */
                pthread_rwlock_rdlock(&rwlock); /*  加锁 */

                int var = counter;
                
//                printf("第%d线程: counter=%d\n", (int )arg, counter);

                /*  解锁 */
                pthread_rwlock_unlock(&rwlock); /*  释放读锁， 一定 */
        }
        
        return NULL;
}


/**
 *  功能描述：  创建N线程， 3个线程写操作， N-3个线程读操作
 *  测试执行：./rwlock
 */
int main(int argc, char *argv[])
{

        /*  初始化读写锁 */
        pthread_rwlock_init(&rwlock, NULL);
        
        /* nt pthread_create(pthread_t *thread, const pthread_attr_t *attr,
           void *(*start_routine) (void *), void *arg);

        */

        pthread_t tid[N];

        /* 创建3线程, 进行写操作 */
        
        int i;
        for (i = 0; i < 3; i++)
                pthread_create(&tid[i], NULL, thread_1, (void *)i);

        /* 再创建N-3， 进行读操作 */
        for (; i < N; i++)
                pthread_create(&tid[i], NULL, thread_2, (void *)i);



        /*  回收资源 */
        void *p; 
        for(i = 0; i <N; i++)
                pthread_join(tid[i], &p);

        /* 主线程打印 counter*/

        printf("counter:%d\n", counter);

        /*  销毁读写锁 */
                pthread_rwlock_destroy(&rwlock);
        return 0;
}
