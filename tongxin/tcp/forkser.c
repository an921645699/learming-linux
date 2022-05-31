#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<assert.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/wait.h>

int socket_init();
void sig_fun(int sig)
{
    wait(NULL);
}
int main()
{
    signal(SIGCHLD,sig_fun);
    int sockfd = socket_init();
    assert(sockfd != -1);

    while(1)
    {
        struct sockaddr_in caddr;
        int len = sizeof(caddr);
        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);
        if(c < 0)
            continue;
        printf("accept = %d\n",c);
        pid_t pid = fork();
        if(pid == -1)
        {
            close(c);
            continue;
        }
        if(pid == 0)
        {          
            while(1)
            {
                char buff[128] = {0};
                int n = recv(c,buff,127,0);
                if(n<=0)
                    break;
                printf("buff(%d) = %s\n",c,buff);
                send(c,"ok",2,0);
            }
                printf("cli(%d) close\n",c);
                close(c);
                exit(0);
        }
        close(c);
    }   
}
int socket_init()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd ==-1)
        return -1;
    
    struct sockaddr_in saddr,caddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6000);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res ==-1)
        return -1;

    res=listen(sockfd,5);
    if(res == -1)
        return -1;
    return sockfd;
}
