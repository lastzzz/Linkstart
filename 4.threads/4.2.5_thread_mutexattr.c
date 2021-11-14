#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


//测试互斥锁类型：
//使用pthread_mutexattr_setpshared设置互斥锁范围
//使用pthread_mutexattr_settype设置互斥锁类型 
// PTHREAD_MUTEX_ERRORCHECK 为检测错误锁，如果上不了锁不会阻塞，而会返回错误。 在此编译器中为PTHREAD_MUTEX_ERRORCHECK_NP
// PTHREAD_MUTEX_NORMAL 为普通互斥锁，上不了锁会处于阻塞态。  在此编译器中没有找到相应的锁，故使用默认锁。
// PTHREAD_MUTEX_RECURSIVE  为递归互斥锁，在同一个线程内可以一起上锁， 该锁维护有一个锁计数器，线程每次上锁会使值加1，解锁则会-1.只有当锁计数器为0时，才会释放该互斥锁。
// 在此编译器中为PTHREAD_MUTEX_RECURSIVE_NP.
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d--", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
} while (0)

int main (int argc, const char *argv[]){
    pthread_mutex_t mutex;
    if (argc < 2){
        printf("Pleaee input mutex type\n");
        return -1;
    }

    pthread_mutexattr_t mutexattr;

    pthread_mutexattr_init(&mutexattr);

    if (!strcmp(argv[1], "error")){
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK_NP);
        printf("set error success\n");
    }
    else if (!strcmp(argv[1], "normal")){
        printf("set normal success\n");
    }
    else if (!strcmp(argv[1], "recursive")){
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE_NP);
        printf("set recursive success\n");
    }
    else {
        printf("input type error\n");
        return -1;
    }

    pthread_mutex_init(&mutex, &mutexattr);

    if (pthread_mutex_lock(&mutex) != 0){
        printf("lock failed\n");
    }
    else {
        printf("lock success\n");
    }


    if (pthread_mutex_lock(&mutex) != 0){
        printf("lock failed\n");
    }
    else {
        printf("lock success\n");
    }


    pthread_mutex_unlock(&mutex);
    pthread_mutex_unlock(&mutex);

    pthread_mutexattr_destroy(&mutexattr);
    pthread_mutex_destroy(&mutex);

    return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行
