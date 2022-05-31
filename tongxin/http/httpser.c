#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<assert.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>

#define PATH "/home/ann/c218/tongxin/http"

int socket_init();
char* get_filename(char buff[]);

int main()
{
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
        
        char buff[512] = {0};
        int n = recv(c,buff,511,0);
        if(n<=0)
            break;
        printf("read:\n%s\n",buff);

        char* filename =get_filename(buff);
        
        if(filename == NULL)
          {   
            send(c,"404",3,0);
            close(c);
            continue;
        }
        printf("访问的资源名字:%s\n",filename);
        char path[128] = {PATH};
        if(strcmp(filename,"/")==0)
        {
            strcat(path,"/index.html");
        }
        else
        {
            strcat(path,filename);
        }
        printf("访问的资源名字:%s\n",path);    
        
        int fd = open(path,O_RDONLY);
        if(fd == -1)
        {
            send(c,"404",3,0);
            close(c);
            continue;
        }
        int size = lseek(fd,0,SEEK_END);
        lseek(fd,0,SEEK_SET);
        char head[256] ={0};
        strcpy(head,"HTTP /1.1 200 OK\r\n");
        strcat(head,"Sever:httpser\r\n");
        sprintf(head+strlen(head),"Content-Length: %d\r\n",size);
        strcat(head,"\r\n");
            
        send(c,head,strlen(head),0);
        printf("head:\n%s\n",head);

        char data[1024]={0};
        int num =0;
        while((num=read(fd,data,1024))>0)
        {
        send(c,data,num,0);
        }

        close(c);   
    }
}


char* get_filename(char buff[])
{
    if(buff==NULL)
        return NULL;
    char* s = strtok(buff," ");
    if( s == NULL)
        return NULL;
    printf("请求方法:%s\n",s);
    s = strtok(NULL," ");

    return s;
}

int socket_init()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd ==-1)
        return -1;
    
    struct sockaddr_in saddr;
    memset(&saddr,0,sizeof(saddr));
    saddr.sin_family=AF_INET;
    saddr.sin_port=htons(80);
    saddr.sin_addr.s_addr=inet_addr("127.0.0.1");

    int res = bind(sockfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(res == -1)
        return -1;

    res=listen(sockfd,5);
    if(res == -1)
        return -1;
    return sockfd;
}
