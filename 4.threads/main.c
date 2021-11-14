#include "pool.h"

int main (int argc, const char *argv[]){
    //线程池中最多三个活动线程
    pool_init(3);

    sleep(1);
    // 连续向池中投入10个任务

    int *workingnum = (int*) malloc (sizeof(int) * 10);

    int i;
    for (i = 0; i < 10; i++){
        workingnum[i] = i;
        pool_add_worker(myprocess, &workingnum[i]);
    }

    sleep(5);

    //摧毁线程池
    pool_destroy();
    free(workingnum);
    return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行
