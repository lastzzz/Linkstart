#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main (int argc, const char* argv[]){
    
    pid_t pid;
    pid = fork();

    if (pid < 0){
        perror("fork error");
        return -1;
    }
    else if (pid == 0){
        printf("The child process, id = %d, parent id = %d\n", getpid(), getppid());
    }
    else {
        printf("The parent process, id = %d\n", getpid());
        while (1);
    }
    return 0;
}