#include<stdio.h>
#include<unistd.h>


// 无名管道的使用
// 测试无名管道的大小和属性
#define N 1024
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d\n", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
}while (0)

int main(int argc, const char *argv[]){
    int i, fd[2];//fd[0] 为管道的读端， fd[1] 为管道的写端。
    int count = 0;
    int nbyte = 0;
    char buf[N] = "";
    for(int i = 0; i < N; ++i){
        buf[i] = 'q';
    }

    if (pipe(fd) < 0){
        errlog("pipe error");
    }

    while (1){
        nbyte = write(fd[1], buf, N);
        count += nbyte;
        printf("count = %d\n", count);//无名管道的大小为65536字节（64kb），当管道被写满后，写操作将会阻塞。
        sleep(1);
    }
    return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行