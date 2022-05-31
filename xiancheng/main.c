#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>

#define SIZE 10
#define SC_NUM 2
#define XF_NUM 3
int buff[SIZE];
pthread_mutex_t mutex;
sem_t sem_empty;
sem_t sem_full;

int in=0;
int out=0;

void* sc_thread(void* arg)
{
    for(int i=0;i<30;i++)
    {
        sem_wait(&sem_empty);
        pthread_mutex_lock(&mutex);
        buff[in] = rand()%100;
        printf("sc in=%d,write=%d\n",in,buff[in]);
        in = (in+1)%SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_full);
        
        sleep(rand()%5);
    }

}
void* xf_thread(void* arg)
{
    for(int i=0;i<20;i++)
    {
        sem_wait(&sem_full);
        pthread_mutex_lock(&mutex);
        printf("--------xf out=%d,read=%d\n",out,buff[out]);
        out = (out+1)%SIZE;
        pthread_mutex_unlock(&mutex);
        sem_post(&sem_empty);
        
        sleep(rand()%5);
    }
}
int main()
{
    pthread_mutex_init(&mutex,NULL);
    sem_init(&sem_empty,0,SIZE);
    sem_init(&sem_full,0,0);
    
    pthread_t id[SC_NUM+XF_NUM];
    for(int i=0;i<SC_NUM;i++)
    {
        pthread_create(&id[i],NULL,sc_thread,NULL);
    }
    for(int i=SC_NUM;i<SC_NUM+XF_NUM;i++)
    {
        pthread_create(&id[i],NULL,xf_thread,NULL);
    }
    for(int i=0;i<SC_NUM+XF_NUM;i++)
    {
        pthread_join(id[i],NULL);
    }
    
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    pthread_mutex_destroy(&mutex);

    exit(0);
}

