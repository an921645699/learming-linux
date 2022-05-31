#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/sem.h>

union semun
{
    int val;
};

void sem_init();
void sem_p();
void sem_v();
void sem_destroy();

