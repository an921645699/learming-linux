#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

sem_t sem1;
sem_t sem2;
sem_t sem3;
void* fun1(void* arg)
{
    for(int i=0;i<5;i++)
    {
        sem_wait(&sem1);//ps1
        printf("A");
        fflush(stdout);
        sem_post(&sem2);//vs2
    }
}

void* fun2(void* arg)
{
    for(int i=0;i<5;i++)
    {
        sem_wait(&sem2);//ps2
        printf("B");
        fflush(stdout);
        sem_post(&sem3);//vs3
    }
{

void* fun3(void* arg)
{
    for(int i=0;i<5;i++)
    {
        sem_wait(sem3);//ps3
        printf("C");
        fflush(stdout);
        sem_post(&sem1);//vs1
    }
}
int main()
{
    sem_init(&sem1,0,1);
    sem_init(&sem2,0,0);
    sem_init(&sem3,0,0);

    pthread_t id1,id2,id3;
    
    pthread_create(&id1,NULL,fun1,NULL);
    pthread_create(&id2,NULL,fun2,NULL);
    pthread_create(&id3,NULL,fun3,NULL);

    pthread_join(id1,NULL);
    pthread_join(id2,NULL);
    pthread_join(id3,NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);
}
