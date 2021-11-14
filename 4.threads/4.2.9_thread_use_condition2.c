#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>


// 使用条件变量，使输入与读取有序。线程1输入，线程2，3轮流输出。
// 本例线程1与之前不同，本例因输入不是采用fgets()函数，故得进行加锁操作，并在线程1中加入计数。
#define N 32
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d--", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
} while (0)

char buf[N] = "";
pthread_cond_t cond;
pthread_mutex_t lock;
int count = 0;

void *thread1_handler(void *arg){
    while (1){
        printf("conut = %d\n", ++count);
        sleep(1);
        pthread_mutex_lock(&lock);
        strcpy(buf, "hello");
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&cond);
    }
    pthread_exit(0);
}

void *thread2_handler(void *arg){
    while (1){
        pthread_mutex_lock(&lock);      // 执行加锁操作
        pthread_cond_wait(&cond, &lock); // 线程执行阻塞，此时自动执行解锁，当线程收到唤醒信号，函数立即返回，此时在进入临界区之前，再一次加锁。
        printf("thread2 buf : %s\n", buf);   // 临界区
        sleep(1);
        pthread_mutex_unlock(&lock);   // 解除互斥锁
    }
    pthread_exit(0);
}

void *thread3_handler(void *arg){
    while (1){
        pthread_mutex_lock(&lock);                  //      此处互斥锁的作用是对pthread_cond_wait()函数的睡眠进行保护，保证在线程的睡眠过程中不会被打断，一旦睡眠成功，pthread_cond_wait()除了实现
        pthread_cond_wait(&cond, &lock);            // 阻塞，还将刚才持有的互斥锁解除，避免死锁发生，不会影响其他线程执行睡眠。
        printf("thread3 buf : %s\n", buf);          //      当线程被执行唤醒操作时，pthrad_cond_wait()函数立即返回，会再次自动执行加锁操作，并进入之后的临界区，操作共享资源。
        sleep(1);                                   // 此时互斥锁的功能为对临界区加锁，保证线程对共享资源操作的完整性。
        pthread_mutex_unlock(&lock);
    }
    pthread_exit(0);
}

int main (int argc, const char *argv[]){
    pthread_t thread1, thread2, thread3;

    if (pthread_cond_init(&cond, NULL) != 0){
        errlog("pthread_cond_init error");
    }

    if (pthread_mutex_init(&lock, NULL) != 0){
        errlog("pthread_mutex_init error");
    }

    if (pthread_create(&thread1, NULL, thread1_handler, NULL) != 0){
        errlog("pthread_create1 error");
    }

    if (pthread_create(&thread2, NULL, thread2_handler, NULL) != 0){
        errlog("pthread_create2 error");
    }

    if (pthread_create(&thread3, NULL, thread3_handler, NULL) != 0){
        errlog("pthread_create3 error");
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);
    return 0;
    
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行

