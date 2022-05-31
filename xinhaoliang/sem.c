#include"sem.h"
#define SEM_NUM 2
static int semid = -1;
void sem_init()
{
    //semget
    //semctl
    semid = semget((key_t)1234,SEM_NUM,IPC_CREAT|IPC_EXCL|0600);
    if(semid==-1)
    {
        semid=semget((key_t)1234,SEM_NUM,0600);
        if(semid == -1)
        {
            printf("create sem failed\n");
            return ;
        }
    }
    else
    {
        union semun a;
        a.val = 1;
        if(semctl(semid,0,SETVAL,a)==-1)
            printf("semctl setval failed\n");
    }
}

void sem_p()
{
    //semop -1
    struct sembuf buf;
    buf.sem_num=0;
    buf.sem_op=-1;
    buf.sem_flg=SEM_UNDO;

    if(semop(semid,&buf,1)==-1)
        printf("semop p err\n");
}
void sem_v()
{
    //semop +1
    struct sembuf buf;
    buf.sem_num=0;
    buf.sem_op=1;
    buf.sem_flg=SEM_UNDO;

    if(semop(semid,&buf,1)==-1)
        printf("semop v err\n");
}
void sem_destroy()
{
    //semctl
    if(semctl(semid,0,IPC_RMID)==-1)
        printf("sem del err\n");
}


