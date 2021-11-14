#include "pool.h"

static CThread_Pool *pool = NULL;

void *thread_routine(void *arg){
    printf("thread start  0x%lx\n", pthread_self());

    while (1){
        pthread_mutex_lock(&(pool->queue_lock));

        // 如果等待队列为0并且不摧毁线程池，则线程处于阻塞状态
        while (pool->cur_queue_size == 0 && !pool->shutdown){
            printf("thread 0x%lx is waiting\n", pthread_self());
            pthread_cond_wait(&(pool->queue_ready), &(pool->queue_lock));
        }

        // 线程池如果摧毁
        if (pool->shutdown){
            pthread_mutex_unlock(&(pool->queue_lock));
            printf("thread 0x%lx will exit\n", pthread_self());
            pthread_exit(NULL);
        }

        printf("thread 0x%lx is starting to work\n", pthread_self());

        assert(pool->cur_queue_size != 0);
        assert(pool->queue_head != NULL);

        // 等待队列长度减1， 并取出链表中的头元素
        pool->cur_queue_size--;
        CThread_Worker *worker = pool->queue_head;
        pool->queue_head = worker->next;

        pthread_mutex_unlock(&(pool->queue_lock));

        // 调用回调函数，并执行任务

        (*(worker->process))(worker->arg);

        free(worker);
        worker = NULL;
    }

    pthread_exit(NULL);
}

void pool_init(int max_thread_num){
    pool = (CThread_Pool*) malloc (sizeof(CThread_Pool));
    
    pthread_mutex_init (&(pool->queue_lock), NULL);
    pthread_cond_init(&(pool->queue_ready), NULL);

    pool->queue_head = NULL;

    pool->max_thread_num = max_thread_num;
    pool->cur_queue_size = 0;

    pool->shutdown = 0;

    pool->threadid = (pthread_t *) malloc(max_thread_num * sizeof(pthread_t));

    int i = 0;
    for (i = 0; i < max_thread_num; ++i){
        pthread_create(&(pool->threadid[i]), NULL, thread_routine, NULL);
    }
}

// 测试代码，添加到线程池的任务
void *myprocess(void *arg){
    printf("threadid is 0x%lx, working on task %d\n", pthread_self(), *(int *)arg);
    sleep(1);
    return NULL;
}

// 向线程池中添加任务

int pool_add_worker(void *(*process) (void *arg), void *arg){

// 构造一个新任务
    CThread_Worker *newworker = (CThread_Worker*) malloc (sizeof(CThread_Worker));
    newworker->process = process;
    newworker->arg = arg;
    newworker->next = NULL;

    pthread_mutex_lock(&(pool->queue_lock));

    // 把任务放到放到等待队列中

    CThread_Worker *member = pool->queue_head;
    if (member != NULL){
        while (member->next != NULL){
            member = member->next;
        }

        member->next = newworker;
    }
    else {
        pool->queue_head = newworker;
    }

    assert(pool->queue_head != NULL);

    pool->cur_queue_size++;

    pthread_mutex_unlock(&(pool->queue_lock));
// 等待队列中有任务，唤醒一个等待线程
    pthread_cond_signal(&(pool->queue_ready));
    
    return 0;
}


//摧毁线程池，等待队列中的任务不会再被执行，但是正在执行的线程会一直把任务运行完后再退出。
int pool_destroy(){
    if (pool->shutdown){
        return -1;
    }
    
    pool->shutdown = 1;

    //唤醒所有等待线程，线程池要摧毁了
    pthread_cond_broadcast(&(pool->queue_ready));

    //阻塞等待线程退出，否则就成僵尸进程了

    int i;

    for (i = 0; i < pool->max_thread_num; i++){
        pthread_join(pool->threadid[i], NULL);
    }

    free(pool->threadid);

    //摧毁等待队列
    CThread_Worker *head = NULL;
    while (pool->queue_head != NULL){
        head = pool->queue_head;
        pool->queue_head = pool->queue_head->next;
        free(head);
    }

    pthread_mutex_destroy(&(pool->queue_lock));
    pthread_cond_destroy(&(pool->queue_ready));

    free(pool);
    pool = NULL;

    return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行

