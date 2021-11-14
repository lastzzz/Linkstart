#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

// 本例在两个printf之间设置断点，使用pthread_testcancel()设置可取消点。
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d\n", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
}while (0)

void *thread_handler(void *arg){
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);

    while (1){
        printf("thread\n");
        pthread_testcancel();
        printf("thread...\n");
    }
    pthread_exit(0);
}

int main(int argc, const char *argv[]){
    pthread_t thread;
    void *retval;

    if (pthread_create(&thread, NULL, thread_handler, NULL) != 0){
        errlog("thread_create error");
    }

    pthread_cancel(thread);

    pthread_join(thread, NULL);
    return 0;
}

// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行