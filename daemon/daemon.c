#include<stdio.h>
#include<unistd.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/stat.h>

void set_daemon()
{
    pid_t pid = fork();
    if(pid != 0)
        exit(0);

    setsid();
    pid = fork();
    if(pid != 0)
        exit(0);

    chdir("/");
    umask(0);
    int maxfd = getdtablesize();
    for(int i = 0;i<maxfd;i++)
    {
        close(i);
    }
}

int main()
{
    set_daemon();

    while(1)
    {
        FILE* fp = fopen("/tmp/daemon.log","a");
        if(fp == NULL)
            break;

        time_t tv;
        time(&tv);
        asctime(localtime(&tv));
        fprintf(fp,"Time is %s\n",asctime(localtime(&tv)));
        fclose(fp);

        sleep(5);
    }
       
}
