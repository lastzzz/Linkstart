#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>

//使用 pthread_detach()来分离线程。
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d--\n", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
}while (0)

void *thread_handler(void *arg){
    pthread_detach(pthread_self());

    int count = *((int *) arg);
    while (count > 0){
        printf("thread...\n");
        sleep(1);
        count--;
    }
    return NULL;
}

int main(int argc, const char *argv[]){
    pthread_t thread;
    int arg = 3;
    if (pthread_create(&thread, NULL, thread_handler, (void *) &arg) != 0){
        errlog("pthread_create1 error");
    }
    
    int temp = 0;
    sleep(2);
    if (pthread_join(thread, NULL) == 0){
        printf("pthread wait success\n");
        temp = 0;
    }
    else {
        printf("pthread wait failed\n");
        temp = 0;
    }
    return temp;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行