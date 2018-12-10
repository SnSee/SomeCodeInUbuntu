

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <pthread.h>

#define NLOOP 1000
#define N 10
#define M 3


#define TIME_  1000*1000


struct baozi{
        int num;                /*  不同馅的包子， 取个随机数 */
        struct baozi *next;
};

struct baozi *head=NULL;

/*  定义互斥锁 */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*  定义条件变量 */
pthread_cond_t  cond;           /*  在函数中初始化 */



/*  生产者线程，  */

void * thread_1(void *arg)    
{


        /*  生产包子 */

        while(1){
                /* 一直生产包子， */
                
                struct baozi  *tmp = malloc(sizeof(*tmp));    /* 有包子皮  */
                tmp->num = rand() % 100; /*  包子馅 */
                tmp->next = NULL;

                printf("包子好了，%d\n", tmp->num);

                /*  把包子， 加入链表 */
                /*  修改head */
                
                pthread_mutex_lock(&mutex);              

                if (head == NULL){
                        head = tmp; /*  头一个包子 */
                }  else {

                        /*  已经有包子， 把新包子放到head位置，     */
                        tmp->next = head;
                        head = tmp;
                }

                pthread_mutex_unlock(&mutex); /*  解锁 */
                
                pthread_cond_signal(&cond); /*  唤醒， 吃包子喽 */
                

                /* 休息一会 */
                usleep(TIME_);
        }
        
      
        return NULL;
}


/* 消费者吃包子 */

void * thread_2(void *arg)    
{

        /*  消费者 */


        while(1)  {
                /*   吃包子 */
                pthread_mutex_lock(&mutex);
                
                while (head == NULL) /*   要用while */
                        pthread_cond_wait(&cond,  &mutex);

                /*  包子准备好了，  */

                struct baozi *tmp  = head;
                head = head->next;

                /*  通知其他线程，吃包子 */
                pthread_cond_broadcast(&cond);
                pthread_mutex_unlock(&mutex);

                /* 吃包子 */
                printf("包子真好吃：%d\n", tmp->num);
                free(tmp);


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
        /* 初始化条件变量 */

        pthread_cond_init(&cond, NULL); /*  采用条件变量默认属性 */
        
        
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

        /* 销毁条件变量 */
        pthread_cond_destroy(&cond);

        return 0;
}
