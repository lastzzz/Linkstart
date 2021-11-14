#ifndef _POOL_H
#define _POOL_H

#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<pthread.h>
#include<assert.h>

// 线程池是所有运行和等待的任务都是一个CTthread_Worker,由于所有任务都在链表里，所以是一个链表结构

typedef struct worker
{
    //回调函数，在任务运行时调用此函数
    void *(*process) (void *arg);
    void *arg;//回调的参数
    struct worker *next;
}CThread_Worker;


// 线程池结构
typedef struct
{
    pthread_mutex_t queue_lock;
    pthread_cond_t queue_ready;

    // 链表结构
    CThread_Worker *queue_head;

    // 是否摧毁线程池

    int shutdown;
    pthread_t *threadid;

    // 线程池中允许活动线程的数量
    int max_thread_num;

    // 当前等待队列的任务数量
    int cur_queue_size;

}CThread_Pool;

extern void pool_init(int max_thread_num);
extern void *thread_routine(void *arg);
extern void *myprocess(void *arg);
extern int pool_add_worker(void *(*process) (void *arg), void *arg);
extern int pool_destroy();

#endif
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行
