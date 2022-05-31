#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<assert.h>

int main()
{
    int fd[2];
    pipe(fd);// 

    pid_t pid =fork();
    assert(pid != -1);
    if(pid==0)
    {
        close(fd[1]);
        while(1)
        {
            char buff[128]={0};
            if(read(fd[0],buff,127)==0)
                break;
            printf("child read:%s\n",buff);
        }
        close(fd[0]);
    }
    else
    {
        close(fd[0]);
        while(1)
        {
            char buff[128]={0};
            fgets(buff,128,stdin);
            if(strncmp(buff,"end",3)==0)
                break;
            write(fd[1],buff,strlen(buff));
        }
        close(fd[1]);
    }
}





