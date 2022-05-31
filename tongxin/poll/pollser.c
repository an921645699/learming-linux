#define _GNU_SOURCE
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<unistd.h>
#include<poll.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define MAX 10
int socket_init();
void poll_fds_init(struct pollfd poll_fds[])
{
    for(int i=0;i<MAX;i++)
    {
        poll_fds[i].fd = -1;
        poll_fds[i].events = 0;
        poll_fds[i].revents = 0;
    }
}
void poll_fds_add(struct pollfd poll_fds[],int fd)
{
    for(int i =0;i<MAX;i++)
    {
        if(poll_fds[i].fd == -1)
        {
            poll_fds[i].fd = fd;
            poll_fds[i].events = POLLIN | POLLRDHUP;
            poll_fds[i].revents = 0;
            break;
        }
    }
}
void poll_fds_del(struct pollfd poll_fds[],int fd)
{
    for(int i=0;i<MAX;i++)
    {
        if(poll_fds[i].fd == fd)
        {
            poll_fds[i].fd = 0;
            poll_fds[i].events = 0;
            poll_fds[i].revents = 0;
            break;
        }
    }
}
int main()
{
    int sockfd = socket_init();
    assert(sockfd != -1);
    
    struct pollfd poll_fds[MAX];
    poll_fds_init(poll_fds);
    poll_fds_add(poll_fds,sockfd);
    
    while(1)
    {
        int n = poll(poll_fds,MAX,5000);
        if(n == -1)
            printf("poll error\n");
        else if(n == 0)
            printf("time out\n");
        else
        {
            for(int i=0;i<MAX;i++)
            {
                if(poll_fds[i].fd == -1)
                    continue;
                if(poll_fds[i].revents & POLLRDHUP)
                {
                    close(poll_fds[i].fd);
                    poll_fds_del(poll_fds,poll_fds[i].fd);
                    printf("cli hup\n");       
                } 

                if(poll_fds[i].revents & POLLIN)
                {
                    if(poll_fds[i].fd == sockfd)
                    {
                        struct sockaddr_in caddr;
                        int len = sizeof(caddr);
                        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);
                        if(c < 0)
                            continue;
                        printf("accept c = %d\n",c);
                        poll_fds_add(poll_fds,c);
                    }
                    else
                    {
                        char buff[128] = {0};
                        int n=recv(poll_fds[i].fd,buff,127,0);
                        if(n <=0 )
                        {
                            printf("cli close");
                            close(poll_fds[i].fd);
                            poll_fds_del(poll_fds,poll_fds[i].fd);
                        }
                        else
                        {
                            printf("recv(%d):%s\n",poll_fds[i].fd,buff);
                            send(poll_fds[i].fd,"ok",2,0);
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
