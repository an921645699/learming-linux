#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<pwd.h>

#define ARGC 10
#define PATH "/home/ann/c218/mybin/"

void print_info()
{
    char* s="$";
    int uid = getuid();
    if(uid == 0)
    {
        s = "#";
    }

    struct passwd* p = getpwuid(uid);
    if(p == NULL)
    {
        printf("$");
        fflush(stdout);
        return;
    }
    
    char hostname[128] = {0};
    gethostname(hostname,128);

    char curr_dir[256] = {0};
    getcwd(curr_dir,256);

    printf("\033[1;32m%s@%s\033[0m \033[1;34m%s\033[0m%s",p->pw_name,hostname,curr_dir,s);
    fflush(stdout);
}

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
        print_info();

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
            if(myargv[1] != NULL)
                if(chdir(myargv[1]) != 0)
                    perror("cd err");
        }        
        else
        {
            pid_t pid =fork();
            if(pid == -1)
                continue;
        
            if(pid == 0)
            {
                //execvp(cmd,myargv);     //execv();

                char path[256] = {0};
                if(strncmp(cmd,"./",2) == 0||strncmp(cmd,"/",1) == 0)
                    strcpy(path,cmd);
                else
                {
                    strcpy(path,PATH);
                    strcat(path,cmd);
                }
                execv(path,myargv);    // /home/ann/c218/mybin/
                printf("execlp err\n");
                exit(0);
            }
        }

        wait(NULL);
    }
    exit(0);

}
