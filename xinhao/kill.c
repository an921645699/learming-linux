#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<string.h>

int main(int argc,char * argv[])
{
    if(argc!=3)
        exit(0);

    int pid = atoi(argv[1]);
    int sig = atoi(argv[2]);
    if(kill(pid,sig)==-1)
        printf("kill err\n");
}
