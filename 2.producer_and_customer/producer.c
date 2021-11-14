#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<sys/file.h>
#include<sys/stat.h>

#define MAXLEN 10 /*缓存区大小最大值*/
#define ALPHABET 1 /*表示使用英文字符*/
#define ALPHABET_START 'a' /*头一个字符*/
#define COUNT_OF_ALPHABET 26 /*字母字符个数*/
#define DIGIT 2 /*表示使用数字*/
#define DIGIT_START '0' /*头一个数字*/
#define COUNT_OF_DIGIT 10 /*数字字符的个数*/
#define SIGN_TYPE ALPHABET /*本例使用英文案例*/
#define N 32
const char* fifo_file = "./2.producer_and_customer/myfifo"; /*仿真FIFO文件名*/
char buf[MAXLEN];

int lock_set(int fd, int type){
    struct flock old_lock, lock;
    lock.l_whence = SEEK_SET;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_type = type;
    lock.l_pid = -1;

    fcntl(fd, F_GETLK, &lock);

    if (lock.l_type != F_UNLCK){
        if (lock.l_type == F_RDLCK){
            printf("Read lock already set by %d\n", lock.l_pid);
        }
        else if (lock.l_type == F_WRLCK){
            printf("Write lock already set by %d\n", lock.l_pid);
        }
    }

    lock.l_type = type;
    
    if ((fcntl(fd, F_SETLKW, &lock)) < 0){
        printf("Lock failed : type = %d\n", lock.l_pid);
        return -1;
    }
    
    switch (lock.l_type)
    {
        case F_RDLCK:
            printf("Read lock set by %d\n", getpid());
            break;
        case F_WRLCK :
            printf("Write lock set by %d\n", getpid());
            break;
        case F_UNLCK:
            printf("Release lock by %d\n", getpid());
            return 1;
            break;
    }

    return 0;
}

int product(void){
    int fd;
    unsigned int sign_type, sign_start, sign_count, size;
    static unsigned int counter = 0;

    if ((fd = open(fifo_file, O_RDWR | O_CREAT | O_APPEND, 0644)) < 0){
        perror("open error");
        return -1;
    }
    
    sign_type = SIGN_TYPE;
    
    switch (sign_type)
    {
        case ALPHABET:
            sign_start = ALPHABET_START;
            sign_count = COUNT_OF_ALPHABET;
        break;
        case DIGIT:
            sign_start = DIGIT_START;
            sign_count = COUNT_OF_DIGIT;
        break;
        default:
            return -1;
    }
    sprintf(buf, "%c", (sign_start + counter));
    counter = (counter + 1) % sign_count;

    lock_set(fd, F_WRLCK);

    if ((size = write(fd, buf, strlen(buf))) < 0){
        perror("Producer : write error");
        return -1;
    }

    lock_set(fd, F_UNLCK);

    close(fd);
    return 0;
}

int main(int argc, const char* argv[]){
    int time_step = 1;
    int time_life = 10;

    if (argc > 1){
        sscanf(argv[1], "%d", &time_step);
    }

    if (argc > 2){
        sscanf(argv[2], "%d", &time_life);
    }

    while (time_life--){
        if (product() < 0){
            break;
        }
        
        sleep(time_step);
    }
    return 0;
}