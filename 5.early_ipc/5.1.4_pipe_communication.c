#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/fcntl.h>



// 无名管道之间的通信
// 本例通过无名管道使父子进程通信，父进程读取文件test1.txt中的数据，并写入管道，子进程读取管道数据，并将数据写入test2.txt。
#define N 128
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d\n", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
}while (0)

int main(int argc, const char *argv[]){
    pid_t pid;
    int fdr, fdw;
    ssize_t nbyte;
    int fd[2];
    char buf[N] = "";
    
    if ((fdr = open("test1.txt", O_RDONLY)) < 0){       // fdr 让父进程来读取test1.txt的内容
        errlog("open read error");
    }

    if ((fdw = open("test2.txt", O_CREAT | O_WRONLY | O_TRUNC, 0664)) < 0){     // fdw 让子进程来将数据写入test2.txt
        errlog("open write error");
    }

    if (pipe(fd) < 0){                      // 建立管道
        errlog("pipe error");
    }

    pid = fork();                           // 建立子进程

    if (pid < 0){
        errlog("fork error");
    }
    else if (pid == 0){
        while ((nbyte = read(fd[0], buf, N)) > 0){          // 子进程： 当管道读有数据时，将数据读取到buf中，并写入test2.txt
            write(fdw, buf, nbyte);
            printf("child\n");
        }
    }
    else{
        while ((nbyte = read(fdr, buf, N)) > 0){            // 父进程 ：当test1.txt有数据时，读取数据到buf中，并写入管道。
            write(fd[1], buf, nbyte);
            printf("parent\n");
        }
    }

    return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行