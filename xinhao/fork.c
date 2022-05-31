#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<signal.h>

void fun(int sig)
{
    printf("sig=%d\n",sig);
    wait(NULL);
}

int main()
{   
    signal(SIGCHLD,fun);
    int n=0;
    char* s=NULL;
    pid_t pid = fork();
    if(pid==-1)
        exit(0);
    if(pid==0)
    {
        n=3;
        s="child";
    }
    else
    {
        n=7;
        s="parent";
    }
    for(int i=0;i<n;i++)
    {
        printf("%s\n",s);
        fflush(stdout);
        sleep(1);
    }
    exit(0);
}
