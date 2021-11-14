#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>

// 有名管道的使用
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d\n", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
}while (0)

int main (int argc, const char *argv[]){
    int fd;
    if (mkfifo("./fifo", 0644) < 0){
        if (errno == EEXIST){               // 当管道文件fifo存在时，无法继续创建，导致mkfifo()函数运行失败，程序收到内核发送的错误码EEXIST表示文件已经存在。
            fd = open("./fifo", O_RDWR);    // 使用O_RDONLY时，函数运行阻塞，但程序并没有退出，而是在打开时阻塞。有名管道在打开时需要注意，如果当使用只读方式打开时，    
            printf("fd = %d\n", fd);        // 打开将会阻塞；直到其他进程使用只写的方式打开同一管道时才会返回，打开操作以读写或只写的方式，将不会阻塞。
        }
        else {
            errlog("mkfifo error");
        }      
    }
    else {
        fd = open("./fifo", O_RDWR);
        printf("fd = %d\n", fd);
    }


    return 0; 
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行