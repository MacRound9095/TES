
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static void *my_func (void *data){
    while(1){
        sleep(10);
    }
}

int main(int argc,char **argv){
    pthread_t tid;
    int ret;
    // 1 创建接收线程
    ret=pthread_create(&tid, NULL,my_func,NULL);
    if(ret){
        printf("create err");
        return 0;
    }
    // 2 主线程读取标准输入，发给接收线程

    
}