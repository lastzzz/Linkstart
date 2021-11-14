#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

// 有名管道通信进程的发送数据端。
#define N 128
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d\n", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
}while (0)

int main(int argc, const char *argv[]){
    int fd;
    char buf[N] = "";

    if (mkfifo("./fifo", 0664) < 0){
        if (errno == EEXIST){
            fd = open("./fifo", O_RDWR);
            printf("fd = %d\n", fd);
        }
        else {
            errlog("mkfifo error");
        }
    }
    else {
        fd = open("./fifo", O_RDWR);
        printf("fd = %d\n", fd);
    }

    while (1){
        fgets(buf, N, stdin);                   // 从标准输入流得到输入，传给buf
        buf[strlen(buf) - 1] = '\0';

        write(fd, buf, strlen(buf));            // 再将buf写入fifo有名管道

        if (strncmp(buf, "quit", 4) == 0){      // 当接收到quit则退出进程。
            break;
        }
    }
    return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行