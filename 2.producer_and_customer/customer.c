#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>

#define MAX_FILE_SIZE 100 * 1024 * 1024 /* 100MB */

const char* fifo_file = "./2.producer_and_customer/myfifo"; /* 仿真FIFO文件名*/
const char* tmp_file = "./2.producer_and_customer/tmp"; /* 临时文件名*/
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
/* 资源消费函数*/
int customing (const char* myfifo, int need){
    int fd;
    char buf;
    int counter = 0;

    if ((fd = open(myfifo, O_RDONLY)) < 0){
        perror("Funct I/On customing error");
        return -1;
    }

    printf("En I/Oy :");
    lseek(fd, SEEK_SET, 0);
    while (counter < need){
        while ((read(fd, &buf, 1) == 1) && (counter < need)){
            fputc(buf, stdout);
            counter++;
        }
    }
    fputs("\n", stdout);

    close(fd);
    return 0;
}

/* 功能 ： 从sour_file文件的offset偏移处开始，将count字节数据复制到dest_file文件 */
int myfilecopy (const char* sour_file, const char* dest_file, int offset, int count, int copy_mode){
    int in_file, out_file;
    int counter = 0;
    char buff_unit;

    if ((in_file = open(sour_file, O_RDONLY | O_NONBLOCK)) < 0){
        perror("Funct I/On myfilecopy error int source file\n");
        return -1;
    }

    if ((out_file = open(dest_file, O_RDWR | O_CREAT | O_TRUNC | O_NONBLOCK, 0664)) < 0){
        perror("Funct I/On myfilecopy error in destinat I/On file");
        return -1;
    }

    lseek(in_file, offset, SEEK_SET);

    while ((read(in_file, &buff_unit, 1) == 1) && (counter < count)){
        write(out_file, &buff_unit, 1);
        counter++;
    }

    close(in_file);
    close(out_file);
    return 0;
}

/* 功能： 实现FIFO消费者 */
int custom (int need){
    int fd;

    customing(fifo_file, need);

    if ((fd = open(fifo_file, O_RDWR)) < 0){
        perror("Funct I/On myfilecopy error in source_file");
        return -1;
    }

    /* 为了模拟FIFO结构，对整个文件内容进行平行移动*/
    lock_set(fd, F_WRLCK);
    myfilecopy(fifo_file, tmp_file, need, MAX_FILE_SIZE, 0);
    myfilecopy(tmp_file, fifo_file, 0, MAX_FILE_SIZE, 0);
    lock_set(fd, F_UNLCK);

    unlink(tmp_file);
    close(fd);

    return 0;
}

int main (int argc, const char* argv[]){
    int customer_capacity = 10;
    
    if (argc > 1){
        sscanf(argv[1], "%d", &customer_capacity);
    }

    if (customer_capacity > 0){
        custom(customer_capacity);
    }
    return 0;
}
