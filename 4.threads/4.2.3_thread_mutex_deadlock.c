#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

// 对线程进行延迟处理，确保线程1先获取互斥锁，并在线程1持有互斥锁期间被取消，此时线程1使用的互斥锁将无法被获取，造成死锁。
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d--", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
} while (0)

int value1 = 0;
int value2 = 0;
int count = 0;
pthread_mutex_t lock;
void *thraed1_handler(void *arg){
    while (1){
        pthread_mutex_lock(&lock);
        value1 = count;
        value2 = count;
        count++;
        sleep(3);
        pthread_mutex_unlock(&lock);
    }
    pthread_exit("thread1...exit");
}

void *thread2_handler(void *arg){
    while (1){
        sleep(1);
        pthread_mutex_lock(&lock);
        if (value1 != value2){
            sleep(1);
            printf("value1 = %d  value2 = %d\n", value1, value2);
        }
        pthread_mutex_unlock(&lock);
    }
    pthread_exit("thread2...exit");
    
}

int main (int argc, const char *argv[]){
    pthread_t thread1, thread2;
    void *retval;
    
    if (pthread_mutex_init(&lock, NULL) != 0){
        errlog("pthread_mutex_init error");
    }

    if (pthread_create(&thread1, NULL, thraed1_handler, NULL) != 0){
        errlog("pthread_create1 error");
    }

    if (pthread_create(&thread2, NULL, thread2_handler, NULL) != 0){
        errlog("pthread_create2 error");
    }

    sleep(2);
    pthread_cancel(thread1);

    pthread_join(thread1, NULL);
    // printf("%s\n", (char*) &retval);

    pthread_join(thread2, NULL);
    // printf("%s\n", (char*) &retval);

    pthread_mutex_destroy(&lock);
    return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行