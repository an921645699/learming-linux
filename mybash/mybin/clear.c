#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{

    printf("\033[2J \033[0:0H");

    exit(0);
}
