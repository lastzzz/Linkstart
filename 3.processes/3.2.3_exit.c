#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main (int argc, const char* argv[]){

    printf("exit befor.....");
    /*  _exit(0); 直接将缓冲区清空 */   
    exit(0);        /* 将缓冲区输出后（刷新I/O缓冲区）退出。*/
    printf("exit after....");
    return 0;
}