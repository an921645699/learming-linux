#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

int main()
{
    int fd=open("fifo",O_RDONLY);
    printf("fd=%d",fd);

    while(1)
    {
        char buff[128]={0};
        int n=read(fd,buff,127);
        if(n==0)
            break;
    
        printf("n=%d buff=%s \n",n,buff);
    }
    close(fd);
    exit(0);
}

