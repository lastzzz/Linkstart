#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

// 有名管道通信进程的接受数据端。
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
            fd = open("./fifo", O_RDWR);        //因在windows下模拟linunx，故文件不能从程序创立，而得自己手动建立。
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
        read(fd, buf, N);                           // 从fifo有名管道读取数据到buf

        if (strncmp(buf, "quit", 4) == 0){          // 收到quit退出进程。
            // system("rm fifo");                   // 退出不删掉fifo，因为程序不能自动建立fifo文件
            break;
        }

        printf("demon A(send) : %s\n", buf);        // 将buf输出
        sleep(1);
    }
    return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行