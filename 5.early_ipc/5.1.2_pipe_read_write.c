#include<stdio.h>
#include<unistd.h>


// 测试无名管道的读写属性
#define N 1024
#define errlog(errmsg) do{\
    perror(errmsg);\
    printf("--%s--%s--%d\n", __FILE__, __FUNCTION__, __LINE__);\
    return -1;\
}while (0)

int main(int argc, const char *argv[]){
   int fd[2];
   int count = 0;
   char buf[N] = "";
   for (int i = 0; i < N; ++i){
       buf[i] = 'q';
   }

   if (pipe(fd) < 0){
       errlog("pipe error");
   }

   while (1){
       write(fd[1], buf, N);
       count++;
       printf("count = %d\n", count);
       if (count == 64){
           read(fd[0], buf, N);//书中写着：无名管道中的数据被写满后，写操作将被阻塞，当管道中出现4kb以上的空闲空间时，可以继续写入4kb的整数倍的数据。
           read(fd[0], buf, N);// 但是现在程序中只要有1kb空闲空间，就会继续写入1kb数据。
        //    read(fd[0], buf, N);
        //    read(fd[0], buf, N);
        //    read(fd[0], buf, N);
        //    read(fd[0], buf, N);
        //    read(fd[0], buf, N);
        //    read(fd[0], buf, N);
       }
   }
   return 0;
}
// 手动编译 gcc 文件名.c -pthread
// ./a.out  运行