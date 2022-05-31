#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
void fun(int sig)
{
    printf("sig=%d\n",sig);
    signal(sig,SIG_DFL);
}

int main()
{
    signal(SIGINT,fun);
    while(1)
    {
        printf("hello\n");
        sleep(1);
    }
     exit(0);
}
