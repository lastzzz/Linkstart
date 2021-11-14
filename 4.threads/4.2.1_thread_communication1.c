#include<stdio.h>
#include<unistd.h>
#include<pthread.h>

#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d--\n", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
}while (0)

int value1 = 0;
int value2 = 0;
int count = 0;

void *thread1_handler(void *arg){
    while (1){
        value1 = count;
        value2 = count;
        count++;
    }
    pthread_exit("thraed1...exit");
}

void *thread2_handler(void *arg){
    while (1){
        if (value1 != value2){
            sleep(1);
            printf("value1 = %d  value2 = %d\n", value1, value2);
        }
    }
    pthread_exit("thread2...exit");
}


int main(int argc, const char *argv[]){
    pthread_t thread1, thread2;
    void *retval;

    if (pthread_create(&thread1, NULL, thread1_handler, NULL) != 0){
        errlog("pthread1_create error");
    }

    if (pthread_create(&thread2, NULL, thread2_handler, NULL) != 0){
        errlog("pthread2_create error");
    }

    pthread_join(thread1, &retval);
    printf("%s\n", (char*) &retval);

    pthread_join(thread2, &retval);
    printf("%s\n", (char*) &retval);

    return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行