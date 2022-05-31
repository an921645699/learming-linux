#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/wait.h>

#define ARGC 10
char* get_cmd(char buff[],char* myargv[])
{
    if(buff == NULL|| myargv == NULL)
        return NULL;

    int i = 0;
    char* s = strtok(buff," ");
    while(s != NULL)
    {
        myargv[i++] = s;
        s = strtok(NULL," ");
    }
    return myargv[0];
}


int main(int argc,char* argv[],char* envp[])
{
    while(1)
    {
        printf("stu@localhost ~$ ");
        fflush(stdout);

        char buff[128]={0};
        fgets(buff,128,stdin);//"ls\n"
        buff[strlen(buff)-1]=0;//"ls"
        
        char* myargv[10]={0};
        char* cmd= get_cmd(buff,myargv);//ps -f, argc[0]=ps, argv[1]=-f 

        if(cmd==NULL)
            continue;

        if(strcmp(cmd,"exit")==0)
        {
            break;
        }
        else if(strcmp(cmd,"cd")==0)
        {
            //
        }        
        else
        {
            pid_t pid =fork();
            if(pid == -1)
                continue;
        
            if(pid == 0)
            {
                execvp(cmd,myargv);
                printf("execlp err\n");
                exit(0);
            }
        }

        wait(NULL);
    }
    exit(0);

}
