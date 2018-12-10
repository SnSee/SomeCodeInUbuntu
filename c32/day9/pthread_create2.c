

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>

#define NLOOP 5
#define N 2
void * thread(void *arg)
{
        int i;

        /*  线程的处理，  */
        
        for (i = 0; i < NLOOP; i++){
                printf("第%d线程\n", (int )arg);
                sleep(1);
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

        /* 编码实现， 对p处理 */
        
        
        return 0;
}
