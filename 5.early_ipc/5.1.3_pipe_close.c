#include<stdio.h>
#include<unistd.h>


// 测试无名管道读端关闭的情况
#define N 1024
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d\n", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
}while (0)

int main (int argc, const char* argv[]){
    int fd[2];
    int count = 0;
    char buf[N] = "";
    for (int i = 0; i < N; ++i){
        buf[i] = 'q';
    }

    if (pipe(fd) < 0){
        errlog("pipe error");
    }

    close(fd[0]);

    write(fd[1], buf, N);   // 当读端关闭后，写数据导致管道破裂，进程收到内核为其发送的信号SIGPIPE，执行信号的默认处理，使进程退出。
                            // 当管道无数据时，读数据将会阻塞；当管道中有数据时，且写端关闭时，读操作可以读取，不会阻塞。
    return 0;
}
//#include<stdio.h>
#include<unistd.h>

// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行
// strace ./a.out 可以看到程序运行的详细细节。
