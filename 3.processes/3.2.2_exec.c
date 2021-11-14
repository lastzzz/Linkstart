#include<stdio.h>
#include<unistd.h>

int main (int argc, const char* argv[]){

    if (execl("/bin/ls", "ls", "-l", ".", NULL) < 0){
        perror("execl error");
        return -1;
    }
    return 0;
}