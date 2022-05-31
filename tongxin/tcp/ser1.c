#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    assert(sockfd!=-1);

    struct sockaddr_in saddr,caddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port = htons(6666);
    saddr.sin_addr.s_addr = inet_addr("192.168.137.32");

    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    assert(res!=-1);

    res = listen(sockfd,5);
    assert(res!=-1);

    while(1)
    {
        int len = sizeof(caddr);
        int c = accept(sockfd,(struct sockaddr*)&caddr,&len);
        if(c<0)
            continue;
        printf("%d\n",c);

        char buff[128] = {0};

        int num = recv(c,buff,127,0);
        printf("buff=%s\n",buff);
        send(c,"ok",2,0);
        
        close(c);

    }


}


