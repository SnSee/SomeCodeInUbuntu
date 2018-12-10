

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <semaphore.h>

#include <pthread.h>

#define NUM 4                   /* 表示资源数量， 对于生产者来说， 表示盘子 */
#define N 2                    /*  先创建N 个线程， N-M个表示消费者线程 */
#define M 1                     /*  前M个表示生产者 */


#define TIME_  1000*1000

int  panzi[NUM];                /* 用于盛包子，  */

/*  定义信号量*/

sem_t  panzi_sem,  baozi_sem;

/*  生产者线程，  */



void * thread_1(void *arg)    
{

int p = 0;
        /*  生产包子 */

        while(1){
                /* 一直生产包子， */

                sem_wait(&panzi_sem); 
                
                panzi[p] = rand() %100 + 1; /*  生产一个包子 */
                printf("包子好了，%d\n", panzi[p]);
                p = (p +1) % NUM;       /*  指定下一个放包子的位置，（下一个盘子）  */

                /*  生产完成一个包子 */

                sem_post(&baozi_sem);

                /* 休息一会 */
                usleep(TIME_);
        }
        
      
        return NULL;
}


/* 消费者吃包子 */

void * thread_2(void *arg)    
{

        /*  消费者 */
        int c = 0;

        while(1)  {
                /*   吃包子 */

                sem_wait(&baozi_sem);  /*  包子资源初始为0 */

                /*  包子准备好了，  */

                /* 吃包子 */
                printf("包子真好吃：%d\n", panzi[c]);

                panzi[c] = 0;
                c = (c + 1)%NUM;

                /*  包子吃完了，意味着盘子空出一个 */
                sem_post(&panzi_sem); /*  */


                usleep(TIME_);

        }
    
        return NULL;
}


/**
 *  功能描述： 用条件变量实现生产者、消费者模型，  生产者，  累加节点， 消费者减少节点。
 *  测试执行：./cond
 */
int main(int argc, char *argv[])
{

        /*  验证ptype */

        struct timespec a;
        /* 初始化信号量 */

        sem_init(&panzi_sem, 0,  NUM);    /* 初始用于盛包子的盘子资源的数量  */
        sem_init(&baozi_sem, 0,  0);      /*  包子资源的数量， 应该为0 */
                
        
        
        /* nt pthread_create(pthread_t *thread, const pthread_attr_t *attr,
           void *(*start_routine) (void *), void *arg);
        */

        pthread_t tid[N];

        /* 创建M线程, 执行生产者 */
        
        int i;
        for (i = 0; i < M; i++)
                pthread_create(&tid[i], NULL, thread_1, NULL);

        /* 再创建N-M， 进行消费操作 */
        for (; i < N; i++)
                pthread_create(&tid[i], NULL, thread_2, NULL);



        /*  回收资源 */
        void *p; 
        for(i = 0; i <N; i++)
                pthread_join(tid[i], &p);

        /* 销毁信号量 */
        sem_destroy(&panzi_sem);
        sem_destroy(&baozi_sem);

        return 0;
}
