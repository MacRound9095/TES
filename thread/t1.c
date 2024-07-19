
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

static char global_buf[999];
static sem_t isem;
static void *my_func (void *data){
    while(1){
        sem_wait(&isem);
        printf("the message are %s \n",global_buf)
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
           fgets(global_buf, 999, FILE *stream);
           sem_post(&isem);
    }
    return 0;

    
}