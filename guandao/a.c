#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<signal.h>

void fun(int sig)
{
    printf("sig=%d\n",sig);
}

int main()
{
    signal(SIGPIPE,fun);
    int fd=open("fifo",O_WRONLY);
    printf("fd=%d\n",fd);
    while(1)
    {
        printf("input:");
        char buff[128]={0};
        fgets(buff,128,stdin);
        if(strncmp(buff,"end",3)==0)
            break;

        write(fd,buff,strlen(buff));
        
    }
    close(fd);
    exit(0);
}
