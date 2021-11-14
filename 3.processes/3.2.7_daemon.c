#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<syslog.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main (int argc, const char* argv[]){

    pid_t pid, sid;
    int i, fd;
    char *buf = (char*)"This is a Daemon\n";
    
    pid = fork();
    if (pid < 0){
        printf("fork error");
        return -1;
    }
    else if (pid > 0){
        exit(0);
    }
    else {
        openlog("daemon_syslog", LOG_PID, LOG_DAEMON);
        if ((sid = setsid()) < 0){
            syslog(LOG_ERR, "%s\n", "setsid");
            exit(1);
        }
        if ((sid = chdir("/")) < 0){
            syslog(LOG_ERR, "%s\n", "chdir");
            exit(1);
        }
        umask(0);
        for (int i = 0; i < getdtablesize(); i++){
            close(i);
        }

        if ((fd = open("/tmp/daemon.log", O_CREAT | O_WRONLY | O_TRUNC, 0600)) < 0){
            syslog(LOG_ERR, "open");
            exit(1);
        }

        syslog(LOG_INFO, "%s\n", "open daemon.log");

        while (1){
            write(fd, buf, strlen(buf));
            sleep(5);
        }
        close(fd);
        closelog();
    }
    return 0;     
}