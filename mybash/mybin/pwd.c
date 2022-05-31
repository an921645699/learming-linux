#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    char path[256] = {0};
    getcwd(path,256);
    printf("%s\n",path);

    exit(0);
}
