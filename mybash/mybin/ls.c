#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<sys/stat.h>
//opendir()
//readdir()
//closedir()

int main()
{
    char path[256]={0};
    getcwd(path,256);

    DIR* p = opendir(path); //打开目录流（当前）
    if(p == NULL)
        return 0;

    struct dirent* s=NULL;
    while((s=readdir(p))!=NULL)     //读目录流，得到一个文件信息，NULL
    {
        if(strncmp(s->d_name,".",1)==0)
            continue;
       
        struct stat st;
        stat(s->d_name,&st);

        if(S_ISDIR(st.st_mode))
            printf("\033[1;34m%s \033[0m",s->d_name);
        else
            if(st.st_mode&(S_IXUSR|S_IXGRP|S_IXOTH))
                printf("\033[1;32%s \033[0m",s->d_name);

            printf("%s ",s->d_name);


}
    printf("\n");

    closedir(p);    //关闭目录流

}
