#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<sys/epoll.h>

#define MAX 10

int socket_init();
void epoll_add(int epfd,int fd)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd =fd;
    
    if(epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&ev) == -1)
        printf("epoll ctl error\n");
}
void epoll_del(int epfd,int fd)
{
    if(epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL) == -1)
        printf("epoll ctl error\n");
}
int main()
{
    int sockfd = socket_init();
    assert(sockfd != -1);

    int epfd = epoll_create(MAX);//创建内核事件表，本质为红黑树
    assert(epfd != -1);

    epoll_add(epfd,sockfd);
    
    struct epoll_event evs[MAX];
    while(1)
    {
        int n = epoll_wait(epfd,evs,MAX,5000);
        if(n == -1)
            printf("epoll wait error\n");
        else if(n == 0)
            printf("time out\n");
        else
        {
            for(int i=0;i<n;i++)
            {
                int fd = evs[i].data.fd;
                if(evs[i].events & EPOLLIN)
                {
                    if(fd == sockfd)
                    {
                        struct sockaddr_in caddr;
                        int len = sizeof(caddr);
                        int c =accept(fd,(struct sockaddr*)&caddr,&len);
                        if(c <=0 )
                            continue;
                        printf("accept c=%d\n",c);
                        epoll_add(epfd,c);
                    }
                    else
                    {
                        char buff[128]={0};
                        int num =recv(fd,buff,127,0);
                        if(num <= 0)
                        {
                            epoll_del(epfd,fd);
                            close(fd);
                            printf("cli close\n");
                        }
                        else
                        {
                            printf("recv(%d):%s\n",fd,buff);
                            send(fd,"ok",2,0);
                        }
                    }
                }
                //if(evs[i].events & EPOLLOUT) 
            }
        }
    }
}
int socket_init()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd ==-1)
        return -1;

    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6000);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    int ser = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ser == -1)
        return -1;

    ser = listen(sockfd,5);
    if(ser == -1)
        return -1;
    return sockfd;
}
