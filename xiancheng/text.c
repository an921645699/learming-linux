#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<pthread.h>

void* thread_fun(void* arg)
{
    int index = (int)arg;
    printf("fun index=%d\n",index);
}

int main()
{
    pthread_t id[5];
    int i=0;
    for(i=0;i<5;i++)
    {
        pthread_create(&id[i],NULL,thread_fun,(void*)i);
           
    }
    for(i=0;i<5;i++)
    {
        pthread_join(id[i],NULL);
    }
    exit(0);
}
