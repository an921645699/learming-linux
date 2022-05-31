#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include"sem.h"

int main()
{
    sem_init();
    srand((int)time(NULL));
    for(int i=0;i<5;i++)
    {
        sem_p();
        printf("A");
        fflush(stdout);
        int n=rand()%3;
        sleep(n);
        printf("A");
        fflush(stdout);
        sem_v();

        n=rand()%3;
        sleep(n);
    }
    sleep(10);
    sem_destroy();
}

