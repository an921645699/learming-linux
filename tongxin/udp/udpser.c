#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<assert.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int socket_init();
int main()
{
    int sockfd = socket_init();
    assert(sockfd != -1);

    while(1)
    {
        struct sockaddr_in caddr;
        int len = sizeof(caddr);
        
        while(1)
        {
            char buff[128] = {0};
            recvfrom(sockfd,buff,128,0,(struct sockaddr*)&caddr,&len);
            
            printf("buff = %s\n",buff);
            sendto(sockfd,"ok",2,0,(struct sockaddr*)&caddr,sizeof(caddr));
        }
    printf("cli close\n");   
    close(sockfd);

    }
}
int socket_init()
{
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd ==-1)
        return -1;
    
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(6000);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res ==-1)
        return -1;

    return sockfd;
}
