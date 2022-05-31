#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAX 10
int socket_init();
void fds_init(int fds[])
{
    for(int i=0;i<MAX;i++)
        fds[i] = -1;
}
void fds_add(int fd,int fds[])
{
    for(int i=0;i<MAX;i++)
    {
        if(fds[i] == -1)
        {
            fds[i] = fd;
            break;
        }
    }
}
void fds_del(int fd,int fds[])
{
    for(int i = 0;i<MAX;i++)
        {
            if(fd == fds[i])
            {
                fds[i] = -1;
                break;
            }
        }
}
int main()
{
    int sockfd = socket_init();
    assert(sockfd != -1);

    int fds[MAX];
    fds_init(fds);
    fds_add(sockfd,fds);
    
    fd_set fdset;
    
    while(1)
    {
        FD_ZERO(&fdset);
        int maxfd = -1;

        for(int i = 0;i<MAX;i++)
        {
            if(fds[i] == -1)
                continue;
            FD_SET(fds[i],&fdset);
            if(maxfd<fds[i])
                maxfd = fds[i];   
        } 

        struct timeval tv = {5,0};
        int n = select(maxfd+1,&fdset,NULL,NULL,&tv);
        if(n < 0)
            printf("select error\n");
        else if(n == 0)
            printf("time out\n");
        else
        {
            for(int i = 0; i<MAX ;i++)
            {
                if(fds[i] == -1)
                    continue;
                if(FD_ISSET(fds[i],&fdset))
                {
                    if(fds[i] == sockfd)
                    {
                        struct sockaddr_in caddr;
                        int len = sizeof(caddr);
                        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);
                        if( c < 0)
                            continue;
                        printf("accept c = %d\n",c);
                        fds_add(c,fds);
                    }
                    else
                    {
                        char buff[128] = {0};
                        int num = recv(fds[i],buff,127,0);
                        if(num ==0)
                        {
                            printf("cli close\n");
                            close(fds[i]);
                            fds_del(fds[i],fds);
                        }
                        else
                        {
                            printf("buff(%d) = %s\n",fds[i],buff);
                            send(fds[i],"ok",2,0);
                        }
                                    
                    }                    
                }
            }
        }
    }
}
int socket_init()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
        return -1;
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(6000);
    saddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res == -1)
        return -1;

    res = listen(sockfd,5);
    if(res == -1)
        return -1;

    return sockfd;
}
