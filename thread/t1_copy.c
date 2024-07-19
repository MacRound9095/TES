
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include<string.h>
// 互斥量：在子线程打印buf内容的时候，如果此时继续输入键盘，可能会导致打印的内容前半部分是之前的，后半部分是刚输入的
// 因此需要定义互斥量，保证子线程在打印内容的时候，主线程不会输入数据到buf
// 用法：一般主要将访问公共变量资源的操作前后加锁，确保在一段时间内，只能有一个线程访问该资源，在这里就是把BUF锁起来

static char global_buf[999];
static char temp_buf[999];
static sem_t isem;
static pthread_mutex_t g_tmutex = PTHREAD_MUTEX_INITIALIZER;
static void *my_func (void *data){
    while(1){
        sem_wait(&isem);
        pthread_mutex_lock(&g_tmutex);
        printf("the message are %s \n",global_buf);
        pthread_mutex_unlock(&g_tmutex);

    }
}

int main(int apsrgc,char **argv){
    pthread_t tid;
    int ret;
    sem_init(&isem, 0, 0);
    // 1 创建接收线程
    ret=pthread_create(&tid, NULL,my_func,NULL);
    if(ret){
        printf("create err");
        return 0;
    }
    // 2 主线程读取标准输入，发给接收线程
    while(1){
            fgets(temp_buf, 999, stdin);
            pthread_mutex_lock(&g_tmutex);
            memcpy(global_buf, temp_buf, 999);
            pthread_mutex_unlock(&g_tmutex);
           sem_post(&isem);
    }
    return 0;

    
}